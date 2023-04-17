/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrings <hrings@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 13:17:10 by hrings            #+#    #+#             */
/*   Updated: 2023/04/17 21:16:25 by hrings           ###   ########.fr       */
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
# include <algorithm>
#define BUFFER_SIZE 65535
#define EOL "\r\n"
#define MAXBODYSIZE 2147483647 //would be 2GB -1, limit on some explorer
#define TIMEOUTTIME 300
#define MAXURISIZE 10000
#define SIZE_MAX_T (size_t)-1
#define MAXVALUESIZE 8000
#endif
