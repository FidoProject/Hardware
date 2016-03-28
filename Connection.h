#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>

class Connection {
public:
	int socketFd;

	Connection();
	~Connection();
	double getReward();
	void disconnect();
	std::string getString();
};


#endif
