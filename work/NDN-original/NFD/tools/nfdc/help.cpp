/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2022,  Regents of the University of California,
 *                           Arizona Board of Regents,
 *                           Colorado State University,
 *                           University Pierre & Marie Curie, Sorbonne University,
 *                           Washington University in St. Louis,
 *                           Beijing Institute of Technology,
 *                           The University of Memphis.
 *
 * This file is part of NFD (Named Data Networking Forwarding Daemon).
 * See AUTHORS.md for complete list of NFD authors and contributors.
 *
 * NFD is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * NFD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * NFD, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "help.hpp"
#include "format-helpers.hpp"

#include <ndn-cxx/util/logger.hpp>

#include <cerrno>
#include <cstring>
#include <unistd.h>

namespace nfd::tools::nfdc {

NDN_LOG_INIT(nfdc.Help);

constexpr int LIST_COMMAND_NAME_COLUMN_WIDTH = 16;

void
helpList(std::ostream& os, const CommandParser& parser, ParseMode mode, std::string_view noun)
{
  os << "nfdc [-h|--help] [-V|--version] [-f|--batch <batch-file>] [<command> [<args>]]\n\n";
  if (noun.empty()) {
    os << "All subcommands:\n";
  }
  else {
    os << "Subcommands starting with " << noun << ":\n";
  }

  std::vector<const CommandDefinition*> commands = parser.listCommands(noun, mode);
  if (commands.empty()) {
    os << "  (none)\n";
    return;
  }

  for (auto def : commands) {
    os << "  " << def->getNoun() << ' ' << def->getVerb() << ' '
       << text::Spaces{static_cast<int>(LIST_COMMAND_NAME_COLUMN_WIDTH -
                       def->getNoun().size() - def->getVerb().size() - 2)}
       << def->getTitle() << '\n';
  }

  os << "\nSee 'nfdc help <command>' to read about a specific subcommand.\n";
}

static void
helpCommand(const std::string& noun, const std::string& verb)
{
  const std::string manpage = "nfdc-" + noun;

  ::execlp("man", "man", manpage.data(), nullptr);
  NDN_LOG_FATAL("Error opening man page for " << manpage << ": " << std::strerror(errno));
}

int
help(std::ostream& os, const CommandParser& parser, std::vector<std::string> args)
{
  const auto helpOpts = {"help"sv, "--help"sv, "-h"sv};
  auto it = std::find_first_of(args.begin(), args.end(), helpOpts.begin(), helpOpts.end());
  if (it == args.end())
    return 2;

  args.erase(it);
  auto noun = args.size() > 0 ? args[0] : "";
  auto verb = args.size() > 1 ? args[1] : "";

  if (noun.empty()) {
    helpList(os, parser);
    return 0;
  }
  else {
    helpCommand(noun, verb); // should not return
    return 1;
  }
}

} // namespace nfd::tools::nfdc
