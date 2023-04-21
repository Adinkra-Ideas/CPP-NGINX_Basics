/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrings <hrings@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:54:06 by hrings            #+#    #+#             */
/*   Updated: 2023/04/21 16:50:57 by hrings           ###   ########.fr       */
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
		Cgi(http::Server* server);
		~Cgi();
		ErrorCode getErrorCode();
		//std::string &getBody();
		void parse_body_for_headers();
		void set_request(Request *request);
		void run_cgi();

	
		Request				*_request;
	private:
		ErrorCode			error_code;
		std::map<std::string, std::string> env_var;
		char**	env;
		char *argv[3];
		std::string file_path;
		std::string working_dir;
		std::string execute_dir;
		http::Server* server;
		void methodCheck();
		void initEnv();
		void executeScript();
		void runGetScript();
		void runPostScript();
		
};

#endif
