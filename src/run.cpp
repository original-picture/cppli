
#include "arg_parsing.h"
#include "documentation.h"

namespace cppli::detail {
        extern bool current_command_is_leaf_;
        extern subcommand_name_t last_subcommand_;

        void run_impl_(int argc, const char* const* const argv) {
        auto parse_ret = detail::parse(argc, argv);

        if(parse_ret.help_command_index && !parse_ret.printed_help) {
            // help command is special. Skip all other execution if it is encountered
            const auto& help_command = parse_ret.subcommands[*parse_ret.help_command_index];
            (detail::subcommand_name_to_func()[help_command.name])(help_command);
        }
        else if(!parse_ret.printed_help) {
            const auto& commands_vec = parse_ret.subcommands;

            #ifdef CPPLI_FULL_ERROR_CHECKING_BEFORE_RUN
            for(const auto& command : commands_vec) { // throws if any errors would occur calling the given commands, without actually calling them
                (detail::subcommand_name_to_error_checking_func()[command.name])(command);
            }
            #endif


            bool runnable_command_found = false;

            detail::current_command_is_leaf_ = (commands_vec.size() < 2);
            if(!detail::subcommand_name_to_docs()[{"MAIN"}].is_namespace) {
                (detail::subcommand_name_to_func()[{"MAIN"}])(commands_vec[0]);
                runnable_command_found = true;
                detail::last_subcommand_ = {};
            }

            for(unsigned i = 1; i < commands_vec.size(); ++i) {
                detail::last_subcommand_ = commands_vec[i-1].name;
                detail::current_command_is_leaf_ = (i == commands_vec.size()-1);
                if((detail::subcommand_name_to_func().contains(commands_vec[i].name))) {
                    runnable_command_found = true;
                    (detail::subcommand_name_to_func()[commands_vec[i].name])(commands_vec[i]);
                }
            }

            if(!runnable_command_found) {
                std::cerr << "The input did not form any runnable commands\n";
                // TODO: print help here?
            }
        }
    }
}