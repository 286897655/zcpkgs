/** 
 * @copyright Copyright © 2020-2025 code by zhaoj
 * 
 * LICENSE
 * 
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

 /**
 * @author zhaoj 286897655@qq.com
 * @brief 
 */

#include "icli.h"
#include <iostream>
#include "strings.h"
namespace zpkg{

icli::icli(const std::string& prompt):prompt_(prompt),running_(false){
    // self help and quit command
    add_commmand("help","show help information,help [command] for more information about command",[this](const std::vector<std::string>& args){
        show_help(args);
    });
    add_commmand("quit","quit!",[this](const std::vector<std::string>& args){
        quit();
    });
}

void icli::add_commmand(const std::string& command,const std::string& help,command_handler command_call){
    commands_[command] = {.handler = command_call,.help = help};
}

void icli::interactive(){
    show_all_help();
    running_ = true;

    while(running_){
        // show prompt
        std::cout << prompt_;
        std::cout.flush();

        // get line of input
        std::string input;
        if(!std::getline(std::cin,input)){
            // eof
            running_ = false;
            break;
        }
        input = strings::trim(input);
        if(input.empty()){
            // no useable input
            continue;
        }
        // split command and args
        // args[0] ->> command
        // args[1] ... [n] ->> command args
        std::vector<std::string> args = strings::split(input," ");
        if(!args.size()){
            // no input command just crlf
            continue;
        }
        // command
        std::string command = args[0];
        if(command.empty()){
            // no input command just crl
            continue;
        }
        // delete first command,the last is arg
        args.erase(args.begin());
        // query command
        auto it = commands_.find(command);
        if(it != commands_.end()){
            try{
                it->second.handler(args);
            }catch(const std::exception& ex){
                std::cout << "Command run error:" << ex.what() << std::endl;
            }
        }else{
            std::cout << "Unknown command: " << command << std::endl;
            std::cout << "Type 'help' for a list of commands." << std::endl;
        }
    }
}

void icli::show_help(const std::vector<std::string>& args){
    if(!args.size()){
        // no args
        // show all help
        show_all_help();
        return;
    }
    // more than one args
    // help xxx command
    std::string command = args[0];
    if(command.empty())
        return;
    
    auto it = commands_.find(command);
    if(it == commands_.end()){
        std::cout << "Unknown command: " << command << std::endl;
        std::cout << "Type 'help' for a list of commands." << std::endl;
        return;
    }

    std::cout << it->second.help << std::endl;
}

void icli::show_all_help(){
    std::cout << "Availiable commands:" << std::endl;
    for(const auto& command : commands_){
        std::cout << command.first << "        " << command.second.help << std::endl;
    }
}

void icli::quit(){
    running_ = false;
}

};//!namespace zpkg