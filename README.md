Aleph Bet is an open source continuation of the _Marathon 2_ game engine. It can play _Marathon_, _Marathon 2_, and _Marathon Infinity_, as well as third-party scenarios for _Marathon 2_, _Marathon Infinity_, and _Aleph One_. It supports Windows, macOS, and most modern UNIX-compatible operating systems (such as Linux).

Aleph Bet is a direct descendent of Aleph One and Aleph Modular, and is expected to incorporate future changes to Aleph One.

# How to play

1. Get [a release of the engine](https://github.com/Aleph-Bet-Marathon/alephbet/releases) (on Linux, you will probably have to [build it yourself](#how-to-build))
2. Get one or more scenarios, perhaps from [the Marathon Open Source release page](https://alephone.lhowon.org/) (make sure to download "standalone game files" and not one of the bundles)
3. Play!

# How to build

- macOS: Use a recent version of XCode to build `PBProjects/AlephBet.xcodeproj`. (Currently, you will experience some friction relating to code signing.)
- Windows: Use Visual Studio to build `VisualStudio/AlephBet.sln`.
- other: `./configure && make && sudo make install`.

This section will be fleshed out soon.

# License

Aleph Bet is copyright ©1994-2024 Bungie Inc., the Aleph One developers, and the Aleph Bet developers.

Aleph Bet is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Aleph Bet is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received [a copy of the GNU General Public License](COPYING) along with this program. If not, see <https://www.gnu.org/licenses/>.

This license notice applies only to the Aleph Bet engine itself, and does not apply to _Marathon_, _Marathon 2_, or _Marathon Infinity_ scenarios and assets, nor to elements of any third-party scenarios.
