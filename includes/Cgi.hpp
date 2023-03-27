/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrings <hrings@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:54:06 by hrings            #+#    #+#             */
/*   Updated: 2023/03/27 14:57:40 by hrings           ###   ########.fr       */
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
		Cgi(const Request &copy);
		~Cgi();
		ErrorCode getErrorCode();
		std::string getBody();
		
		
	private:
		Request				_request;
		ErrorCode			error_code;
		std::string			body;
		std::map<std::string, std::string> env_var;
		
		void pathCheck();
		void methodCheck();
		void initEnv();
		void executeScript();
		
};

#endif