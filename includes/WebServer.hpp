/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrings <hrings@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 13:17:10 by hrings            #+#    #+#             */
/*   Updated: 2023/04/03 12:54:58 by hrings           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

#include <vector>
# include <iostream>
# include <utility>
# include <string>
# include <sstream>
# include <unistd.h>
# include <fcntl.h>
# include <cstdlib>
# include "Server.hpp"
# include <sys/time.h>
# include <limits.h>
#define BUFFER_SIZE 2097152
#define EOL "\r\n"
#define MAXBODYSIZE 2147483647 //would be 2GB -1, limit on some explorer
#define TIMEOUTTIME 50
#endif