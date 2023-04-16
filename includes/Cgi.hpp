/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrings <hrings@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:54:06 by hrings            #+#    #+#             */
/*   Updated: 2023/04/11 15:57:45 by hrings           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

#include "Request.hpp"
#include <sys/wait.h>
#include <string.h>
#include <iostream>
#include <fstream>
# include "utils.hpp"

class Cgi
{
	public:
		Cgi(std::string root);
		~Cgi();
		ErrorCode getErrorCode();
		std::string &getBody();
		void parse_body_for_headers();
		void set_request(Request *request);
		void run_cgi();

	private:
		Request				*_request;
		ErrorCode			error_code;
		std::string			body;
		std::map<std::string, std::string> env_var;
		char**	env;
		char *argv[3];
		std::string file_path;
		std::string working_dir;
		std::string execute_dir;
		std::string server_root;
		void methodCheck();
		void initEnv();
		void executeScript();
		void runGetScript();
		void runPostScript();
		
};

#endif
