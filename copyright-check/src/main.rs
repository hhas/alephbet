use std::{
    fs::{read_to_string, File},
    io::Write,
    path::{Path, PathBuf},
    sync::OnceLock,
    time::{Duration, Instant},
};

use anyhow::{anyhow, Context};
use clap::Parser;
use liso::{liso, InputOutput};
use regex::{Captures, Regex, RegexBuilder};

#[allow(unused)] // WIP code borrowed from another project, not fully utilized
mod rsync_pattern;
use rsync_pattern::RsyncPattern;

/// Interval at which to display status information (iff the terminal is
/// interactive)
const STATUS_INTERVAL: Duration = Duration::from_millis(100);

#[derive(Parser)]
struct Invocation {
    /// A name identifying the copyrighted work.
    #[clap(long)]
    work: String,
    /// The year(s) and/or year range(s) of the copyright.
    #[clap(long)]
    years: String,
    /// The names of the copyright holders. All whitespace should take the form
    /// of a single space, and the trailing period should not be included.
    #[clap(long)]
    authors: String,
    /// Files whose copyright notices should be checked. (Not subjected to
    /// --include or --exclude.)
    files: Vec<PathBuf>,
    /// A directory whose contents should be checked, subject to --exclude and
    /// --include.
    #[clap(short, long)]
    dir: Vec<PathBuf>,
    /// An rsync-style glob denoting a file that should *not* be checked.
    #[clap(short, long)]
    exclude: Vec<RsyncPattern>,
    /// An rsync-style glob denoting a file that should be checked, even if it
    /// matches a glob passed to --exclude.
    #[clap(short, long)]
    include: Vec<RsyncPattern>,
}

/// A regex matching the form of the standard copyright string.
static COPYRIGHT_FORM: OnceLock<Regex> = OnceLock::new();
fn get_copyright_form() -> &'static Regex {
    COPYRIGHT_FORM.get_or_init(|| {
        // note: if you change this, you will also need to update the year
        // update logic
        RegexBuilder::new(
            r#"\n([-#;/* \t]*)([^\r\n]*) is copyright ©([-0-9., ]+) (.*?)\.$"#,
        )
        .multi_line(true)
        .dot_matches_new_line(true)
        .build()
        .unwrap()
    })
}

/// A regex matching possible nonstandard copyright strings.
static COPYRIGHT_SPUR: OnceLock<Regex> = OnceLock::new();
fn get_copyright_spur() -> &'static Regex {
    COPYRIGHT_SPUR.get_or_init(|| {
        RegexBuilder::new(
            r#"(?:COPY|Copy|copy)-?\s?(?:RIGHT|Right|right)\s*(?:©|[0-9])"#,
        )
        .build()
        .unwrap()
    })
}

/// Compares two strings, except that any time there is a literal ASCII space
/// character in the "prototype", it may match a somewhat loosely-defined
/// sequence of whitespace in the "practice".
fn prototype_matches_practice(prototype: &str, practice: &str) -> bool {
    let mut practice_chars = practice.chars().peekable();
    for protochar in prototype.chars() {
        if protochar == ' ' {
            // true if we have seen nothing but whitespace since the last line
            // break
            let mut line_broken = false;
            // true if we have seen comment characters after a line break
            let mut comment_seen = false;
            // true if we have seen ANY whitespace characters at all, including
            // line breaks and comment characters
            let mut any_whitespace_seen = false;
            loop {
                let Some(next) = practice_chars.peek().copied() else {
                    return false;
                };
                if next == '\r' || next == '\n' {
                    // very permissive in what we consider a linebreak...
                    line_broken = true;
                    comment_seen = false;
                } else if next.is_ascii_whitespace() {
                    // we're okay
                    if comment_seen {
                        line_broken = false;
                        comment_seen = false;
                    }
                } else if line_broken && "-#;/*".contains(next) {
                    comment_seen = true;
                } else {
                    // it's a non-whitespace char, STOP!
                    break;
                }
                // discard the peeked char
                practice_chars.next();
                // if we got here, it counted as "anything"
                any_whitespace_seen = true;
            }
            if !any_whitespace_seen {
                return false;
            }
        } else if practice_chars.next() != Some(protochar) {
            return false;
        }
    }
    practice_chars.next().is_none()
}

struct CheckState {
    next_status_time: Instant,
    total: u64,
    passed: u64,
    // can't use `failures.len()` because we may add support for multiple
    // failures per file
    failed: u64,
    failures: Vec<(String, PathBuf)>,
}

fn inner_check_file(
    invocation: &Invocation,
    path: &Path,
) -> anyhow::Result<()> {
    let contents = read_to_string(path)?;
    let mut matches = get_copyright_form().captures_iter(&contents);
    let first_notice = matches
        .next()
        .ok_or_else(|| anyhow!("Standard copyright notice not found."))?;
    if matches.next().is_some() {
        return Err(anyhow!("Multiple standard copyright notices found."));
    }
    if get_copyright_spur().find_iter(&contents).count() != 1 {
        return Err(anyhow!(
            "Possible spurious/nonstandard copyright notice found."
        ));
    }
    let found_work = &first_notice[2];
    let found_years = &first_notice[3];
    let found_authors = &first_notice[4];
    if found_work != invocation.work {
        liso::println!(format!("{:?} != {:?}", invocation.work, found_work));
        return Err(anyhow!("Copyright notice names an incorrect work."));
    }
    if !prototype_matches_practice(&invocation.authors, found_authors) {
        liso::println!(format!(
            "{:?} != {:?}",
            invocation.authors, found_authors
        ));
        return Err(anyhow!("Copyright notice names incorrect authors."));
    }
    if found_years != invocation.years {
        liso::wrapln!(format!("Updating copyright year for {path:?}"));
        let new_contents =
            get_copyright_form().replace(&contents, |captures: &Captures| {
                assert_eq!(&captures[2], invocation.work);
                assert_ne!(&captures[3], invocation.years);
                // Authors may include weird newlines and comment chars and
                // such. Preserve them. If manual line breaking needs to be
                // updated, a routine commit review should catch that, and
                // manual intervention can be applied.
                format!(
                    "{space}{work} is copyright ©{years} {authors}.",
                    space = &captures[1],
                    work = invocation.work,
                    years = invocation.years,
                    authors = &captures[4],
                )
            });
        assert_ne!(contents, new_contents);
        let mut backup_path = path.to_owned().into_os_string();
        backup_path.push("~");
        let backup_path: PathBuf = backup_path.into();
        let mut neo_path = path.to_owned().into_os_string();
        neo_path.push("!");
        let neo_path: PathBuf = neo_path.into();
        let mut f =
            File::create(&neo_path).context("unable to create new file")?;
        f.write_all(new_contents.as_bytes())
            .context("unable to write new contents to file")?;
        drop(f);
        std::fs::rename(path, &backup_path)
            .context("unable to back up original contents")?;
        std::fs::rename(&neo_path, path)
            .context("unable to move new contents into place")?;
        std::fs::remove_file(&backup_path)
            .context("unable to delete original file")?;
    }
    Ok(())
}

fn check_file(
    invocation: &Invocation,
    path: &Path,
    check_state: &mut CheckState,
) {
    let now = Instant::now();
    if now >= check_state.next_status_time {
        check_state.next_status_time += STATUS_INTERVAL;
        if now >= check_state.next_status_time {
            check_state.next_status_time = now + STATUS_INTERVAL;
        }
        if check_state.failed > 0 {
            liso::output().status(Some(liso!(fg=red, +bold, format!(
                "Checked {} files... ({} passed, {} failed)",
                check_state.total,
                check_state.passed,
                check_state.failed,
            ))))
        } else {
            liso::output().status(Some(liso!(format!(
                "Checked {} files... (so far so good)",
                check_state.total
            ))))
        }
    }
    check_state.total += 1;
    match inner_check_file(invocation, path) {
        Ok(_) => check_state.passed += 1,
        Err(x) => {
            check_state.failed += 1;
            check_state.failures.push((x.to_string(), path.to_owned()));
        }
    }
}

fn check_dir(
    invocation: &Invocation,
    path_buf: &mut PathBuf,
    check_state: &mut CheckState,
) {
    for dent in std::fs::read_dir(&path_buf).unwrap_or_else(|x| {
        panic!("Unable to descend into directory {path_buf:?}: {x}")
    }) {
        let dent = dent.unwrap_or_else(|x| {
            panic!("Unable to read from directory {path_buf:?}: {x}")
        });
        let filename = dent.file_name();
        if filename == "." || filename == ".." {
            continue;
        }
        path_buf.push(&filename);
        if !invocation
            .exclude
            .iter()
            .any(|glob| glob.matches(path_buf.as_os_str().as_encoded_bytes()))
            || invocation.include.iter().any(|glob| {
                glob.matches(path_buf.as_os_str().as_encoded_bytes())
            })
        {
            if std::fs::metadata(&path_buf)
                .unwrap_or_else(|x| {
                    panic!("Unable to get metadata for {path_buf:?}: {x}")
                })
                .is_dir()
            {
                check_dir(invocation, path_buf, check_state);
            } else {
                check_file(invocation, path_buf, check_state);
            }
        }
        // clean up for the next one
        assert!(path_buf.pop());
    }
}

fn main() {
    let invocation = Invocation::parse();
    let io = InputOutput::new();
    let mut check_state = CheckState {
        next_status_time: Instant::now() + STATUS_INTERVAL,
        total: 0,
        passed: 0,
        failed: 0,
        failures: vec![],
    };
    for file in invocation.files.iter() {
        check_file(&invocation, file, &mut check_state);
    }
    for dir in invocation.dir.iter() {
        check_dir(&invocation, &mut dir.to_path_buf(), &mut check_state);
    }
    io.status::<&str>(None);
    if check_state.failures.is_empty() {
        // OK
    } else {
        check_state.failures.sort();
        check_state.failures.dedup();
        let mut last_failure_string = None;
        for (failure_string, failure_path) in check_state.failures.iter() {
            if Some(failure_string) != last_failure_string {
                last_failure_string = Some(failure_string);
                io.wrapln(liso!(fg=red,+bold,failure_string));
            }
            io.wrapln(liso!(fg = red, format!("    {failure_path:?}")));
        }
        io.wrapln(format!(
            "{} files passed, {} failed, out of {} checked.",
            check_state.passed, check_state.failed, check_state.total,
        ));
        // I wish rust already supported `main() -> i32` outside nightly
        io.blocking_die();
        std::process::exit(1)
    }
}

#[cfg(test)]
mod test {
    use super::*;
    #[test]
    fn prototypes_and_practice() {
        const PROTOTYPE: &str = "Sonic, Tails, Knuckles, Amy, and Sticks";
        const PRACTICES: &[&str] = &[
            "Sonic, Tails, Knuckles, Amy, and Sticks",
            "Sonic, Tails,       Knuckles, Amy, and Sticks",
            "Sonic, Tails, Knuckles,\tAmy, and Sticks",
            "Sonic, Tails,\n#Knuckles, Amy, and Sticks",
            "Sonic, Tails, Knuckles,\n # Amy, and Sticks",
            "Sonic, Tails, Knuckles, Amy, and\n *  Sticks",
            "Sonic,\nTails, Knuckles, Amy, and Sticks",
            "Sonic,\n###Tails, Knuckles, Amy, and Sticks",
            "Sonic, Tails,\r###Knuckles, Amy, and Sticks",
        ];
        let mut bad = false;
        for practice in PRACTICES.iter().copied() {
            if !prototype_matches_practice(PROTOTYPE, practice) {
                eprintln!("{:?} != {:?}", PROTOTYPE, practice);
                bad = true;
            }
        }
        if bad {
            panic!("Some prototypes that should match practice did not!");
        }
    }
    #[test]
    fn prototypes_and_bad_practice() {
        const PROTOTYPE: &str = "Sonic, Tails, Knuckles, Amy, and Sticks";
        const PRACTICES: &[&str] = &[
            "Sonic, Tails,Knuckles, Amy, and Sticks",
            "Sonic, Tails, Knuckles,\u{2028}Amy, and Sticks",
            "Sonic, Tails, Knuckles, A my, and Sticks",
            "Sonic, Tails,\n# # Knuckles, Amy, and Sticks",
            "Sonic, Tails, Knuckles,\n / * Amy, and Sticks",
        ];
        let mut bad = false;
        for practice in PRACTICES.iter().copied() {
            if prototype_matches_practice(PROTOTYPE, practice) {
                eprintln!("{:?} == {:?}", PROTOTYPE, practice);
                bad = true;
            }
        }
        if bad {
            panic!("Some prototypes that should not match practice did!");
        }
    }
}
