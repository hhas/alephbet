/*
 *
 *  Aleph Bet is copyright ©1994-2024 Bungie Inc., the Aleph One developers,
 *  and the Aleph Bet developers.
 *
 *  Aleph Bet is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 *
 *  Aleph Bet is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 *  This license notice applies only to the Aleph Bet engine itself, and
 *  does not apply to Marathon, Marathon 2, or Marathon Infinity scenarios
 *  and assets, nor to elements of any third-party scenarios.
 *
 */

#import <Cocoa/Cocoa.h>
#include "cstypes.hpp"
#include "csalerts.hpp"

void system_alert_user(const char* message, short severity)
{
	NSAlert *alert = [NSAlert new];
	if (severity == infoError) 
	{
		[alert setMessageText: @"Warning"];
		[alert setAlertStyle: NSWarningAlertStyle];
	}
	else
	{
		[alert setMessageText: @"Error"];
		[alert setAlertStyle: NSCriticalAlertStyle];
	}
	[alert setInformativeText: [NSString stringWithUTF8String: message]];
	[alert runModal];
	[alert release];
}

bool system_alert_choose_scenario(char *chosen_dir)
{
	NSOpenPanel *panel = [NSOpenPanel openPanel];
	[panel setCanChooseFiles:NO];
	[panel setCanChooseDirectories:YES];
	[panel setAllowsMultipleSelection:NO];
	[panel setTitle:@"Choose Scenario"];
	[panel setMessage:@"Select a scenario to play:"];
	[panel setPrompt:@"Choose"];
	
	if (!chosen_dir)
		return false;
	
	if ([panel runModal] != NSFileHandlingPanelOKButton)
		return false;
	
	return [[[panel URL] path] getCString:chosen_dir maxLength:256 encoding:NSUTF8StringEncoding];
}

void system_launch_url_in_browser(const char *url)
{
	NSURL *urlref = [NSURL URLWithString:[NSString stringWithUTF8String:url]];
	[[NSWorkspace sharedWorkspace] openURL:urlref];
}
