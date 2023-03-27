/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrings <hrings@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:56:47 by hrings            #+#    #+#             */
/*   Updated: 2023/03/27 20:05:17 by hrings           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

Cgi::Cgi(const Request &copy) : _request(copy), error_code(NONE), body("")
{
	methodCheck();
	if(!this->error_code)
		pathCheck();
	if(!this->error_code)
		initEnv();
	if(!this->error_code)
		executeScript();
	
}
Cgi::~Cgi()
{
	
}

ErrorCode Cgi::getErrorCode()
{
	return (this->error_code);
}
std::string Cgi::getBody()
{
	return (this->body);
}

void Cgi::pathCheck()
{
	std::cout << "checking path" << std::endl;
}
void Cgi::methodCheck()
{
	if (!(this->_request.readMethod() == GET || this->_request.readMethod() == POST))
		this->error_code = METHODNOTALLOWED;
}
void Cgi::initEnv()
{
	char *cwd = getcwd(NULL, 0);
  	if (!cwd)
    	return ;
  	std::string cwd_ = cwd;
  	free(cwd);
	if (this->_request.readMethod() == GET)
		this->env_var["REQUEST_METHOD"] = "GET";
	if (this->_request.readMethod() == POST)
		this->env_var["REQUEST_METHOD"] = "POST";
	this->env_var["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->env_var["PATH_INFO"] = this->_request.readPath();
	this->env_var["PATH_TRANSLATED"] = cwd_ + this->_request.readPath();
	this->env_var["REQUEST_URI"] = this->_request.readPath();
	this->env_var["SCRIPT_NAME"] = this->_request.readPath();
	this->env_var["QUERY_STRING"] = this->_request.readQuery();
}

void Cgi::executeScript()
{
	char *argv[2];
	char **env = NULL;

	//TODO memory leak fix on fail
	env = (char **)calloc(sizeof(char *), this->env_var.size() + 1);
	std::map<std::string, std::string>::iterator it = this->env_var.begin();
	for (int i = 0; it != this->env_var.end(); ++it, ++i)
	{
		std::string tmp = it->first + "=" +it->second;
		env[i] = strdup(tmp.c_str());
	}
	size_t index = this->_request.readPath().find_last_of("/");
  	argv[0] = strdup(this->_request.readPath().substr(index + 1).c_str());
  	argv[1] = NULL;
	char *cwd = getcwd(NULL, 0);
  	if (!cwd)
    	return ;
  	std::string cwd_ = cwd;
  	free(cwd);
	std::string file_path = cwd_ + _request.readPath();
	int pip[2];
	pipe(pip);
	pid_t pid = fork();
	if (pid== 0)
	{
		char *cwd = getcwd(NULL, 0);
  		if (!cwd)
    		return ;
  		std::string cwd_ = cwd;
  		free(cwd);
		std::string tmp_file(cwd_ + "/cgi-bin/tmp");
		int tmp_fd;
		mode_t	mode;
		mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
		tmp_fd = open(tmp_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, mode);
		if (tmp_fd < 0)
		{
			http::print_status(ft_RED, "open file error write");
			exit(1);
		}
		close(pip[0]);
		dup2(tmp_fd,1);
		
		execve(file_path.c_str(), argv, env);
		for(int i = 0; env[i]; ++i)
			free(env[i]);
		free(env);
		free(argv[0]);
		exit(1);
	}
	else
	{
		for(int i = 0; env[i]; ++i)
			free(env[i]);
		free(env);
		free(argv[0]);
		close(pip[0]);
		close(pip[1]);
		time_t start = time(NULL);
		int status;
		int wait_return;
		do{
			wait_return = waitpid(pid, &status, WNOHANG);
			if (wait_return == -1)
			{
				http::print_status(ft_RED, "waitpid error");
			}
			if (wait_return)
			{
				break ;
			}
		}while(difftime(time(NULL), start) < TIMEOUTTIME);
		std::cout << "status: " << WEXITSTATUS(status) << std::endl;
		if (WEXITSTATUS(status)) {
			http::print_status(ft_RED, "execusion of cgi script failed");
			this->error_code = INTERNALSERVERERROR;
			return ;
        }
		std::string tmp_file(cwd_ + "/cgi-bin/tmp");
		int tmp_fd;
		tmp_fd = open(tmp_file.c_str(), O_RDONLY);
		if (tmp_fd < 0)
		{
			http::print_status(ft_RED, "open file error read");
			return ;
		}
		lseek(tmp_fd, 0, SEEK_SET);
		while (1)
		{	
			int bytes_read;
			char readbuffer[4096 + 1];
			memset (readbuffer,0,4096);
			
			bytes_read = read(tmp_fd, readbuffer, 4096);
			if (bytes_read <= 0)
			{
				close(tmp_fd);
				std::remove(tmp_file.c_str());
				break ;
			}				
			this->body += std::string(readbuffer);	
			memset (readbuffer,0,80);
			
		}
	}
}
