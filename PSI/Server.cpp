//============================================================================
// Name        : Server.cpp
// Author      : Schejbal Ondrej
// Version     : 1.1 - Sablona pro komunikaci prevzana z 2. proseminare Viktora Cerneho
// Description : 1 PSI Task 2019
// compile with g++ -o server Server.cpp -std=gnu++11 -Wall -pedantic
//============================================================================
#include <iostream>
using namespace std;

#include <cstdlib>
#include <cstdio>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <string>
#include <sstream>
#include <cmath>
#include <queue>

#define BUFFER_SIZE 10240
#define TIMEOUT 1
#define TIMEOUT_RECHARGING 5

struct timeval tv;
fd_set sockets;

enum Direction {LEFT, RIGHT, DOWN, UP};

enum ServerMSG {SERVER_MOVE, SERVER_TURN_LEFT, SERVER_TURN_RIGHT, SERVER_PICK_UP, SERVER_LOGOUT,
				SERVER_OK, SERVER_LOGIN_FAILED, SERVER_SYNTAX_ERROR, SERVER_LOGIC_ERROR};

bool sendServerResponse(int & c, ServerMSG command) {
	string msgForServer = "";
	switch (command) {
		case SERVER_MOVE : {
			msgForServer = "102 MOVE\a\b";
			break;
		}
		case SERVER_TURN_LEFT : {
			msgForServer = "103 TURN LEFT\a\b";
			break;
		}
		case SERVER_TURN_RIGHT : {
			msgForServer = "104 TURN RIGHT\a\b";
			break;
		}case SERVER_PICK_UP : {
			msgForServer = "105 GET MESSAGE\a\b";
			break;
		}case SERVER_LOGOUT : {
			msgForServer = "106 LOGOUT\a\b";
			break;
		}case SERVER_OK : {
			msgForServer = "200 OK\a\b";
			break;
		}case SERVER_LOGIN_FAILED : {
			msgForServer = "300 LOGIN FAILED\a\b";
			break;
		}case SERVER_SYNTAX_ERROR : {
			msgForServer = "301 SYNTAX ERROR\a\b";
			break;
		}case SERVER_LOGIC_ERROR : {
			msgForServer = "302 LOGIC ERROR\a\b";
			break;
		}
		default : {
			cout << "FATAL ERROR 1" << endl;
			close(c);
			exit(1);
		}
	}
	if (send(c, msgForServer.c_str(), msgForServer.size(), 0) < 0) {
		perror("Nemohu odeslat command!");
		close(c);
		return true;
	}
	return false;
}

bool isRechargingResponseSyntaxOK(const string & moveMsg) {
	return !moveMsg.compare(string{"RECHARGING\a\b"});
}

bool isMoveResponseSyntaxOK(const string & moveMsg) {
	//the message has to have \a\b tag after second number
	if (moveMsg[moveMsg.size()-3] > 57 || moveMsg[moveMsg.size()-3] < 48) {
		return false;
	}

	stringstream ss;
	ss << moveMsg;
	string temp = "";
	double number;
	float intNumber;

	ss >> temp;
	if (temp.compare(string{"OK"})) {
		return false;
	}
	temp = "";
	//prvni cislo
	ss >> temp;
	if (!(stringstream(temp) >> number)) {
		return false;
	}
	if (modf(number, &intNumber) != 0.0) {
		return false;
	}
	temp == "";
	//druhe cislo
	ss >> temp;
	if (!(stringstream(temp) >> number)) {
		return false;
	}
	if (modf(number, &intNumber) != 0.0) {
		return false;
	}
	return true;
}

//receives single msg
bool receiveMsg(int & c, char * buffer, int & bytesRead) {
    bytesRead = recv(c, buffer, BUFFER_SIZE, 0);
    if(bytesRead <= 0){
		cout << "zaviram pripojeni" << endl;
        return true;
    }
    buffer[bytesRead] = '\0';

    return false;
}

bool receiveFullMessage(int & c, char * fullMsgBuffer, int & fullBufferLength, const int maxLength, queue<string> & stashedMessages) {
	fullBufferLength = 0;
	//pokud je neco v queue
	if (!stashedMessages.empty()) {
		string message = stashedMessages.front();
		stashedMessages.pop();
		//recharging
		if (message.size() == 12 && isRechargingResponseSyntaxOK(message)) {
			char fullPowerBuffer[BUFFER_SIZE];
			int fullPowerBufferLength = 0;
			tv.tv_sec = TIMEOUT_RECHARGING;
			select(c + 1, &sockets, NULL, NULL, &tv);
			if (receiveFullMessage(c, fullPowerBuffer, fullPowerBufferLength, 12, stashedMessages)) {
				return true;
			}
			tv.tv_sec = TIMEOUT;
			string fullPowerMsg = fullPowerBuffer;
			if (fullPowerMsg.compare(string{"FULL POWER\a\b"})) {
				sendServerResponse(c, ServerMSG::SERVER_LOGIC_ERROR);
				close(c);
				return true;
			}
			return receiveFullMessage(c, fullMsgBuffer, fullBufferLength, maxLength, stashedMessages);
		}
		if (message.size() > (unsigned)maxLength) {
			sendServerResponse(c, ServerMSG::SERVER_SYNTAX_ERROR);
			close(c);
			return true;
		}
		if (maxLength == 7) {
			for (unsigned i = 0 ; i < message.size()-2 ; i++) {
				if (message[i] > 57 || message[i] < 48) {
					sendServerResponse(c, ServerMSG::SERVER_SYNTAX_ERROR);
					close(c);
					return true;
				}
			}
		}
		for (char &c : message) {
			fullMsgBuffer[fullBufferLength++] = c;
		}
		fullMsgBuffer[fullBufferLength] = '\0';
		fullBufferLength = message.size();
		return false;
	}

	char buffer[BUFFER_SIZE];
	int bytesRead = 0;
	//nacteni casti/cele zpravy
	while (bytesRead == 0 || (fullMsgBuffer[fullBufferLength-2] != '\a' || fullMsgBuffer[fullBufferLength-1] != '\b')) {
		if (receiveMsg(c, buffer, bytesRead)) {
        	close(c);
			return true;
		}
		for (int i = 0 ; i < bytesRead ; i++) {
			fullMsgBuffer[fullBufferLength] = buffer[i];
			fullBufferLength++;
		}
		fullMsgBuffer[fullBufferLength] = '\0';
		if (buffer[bytesRead-1] == '\0' && buffer[bytesRead-3] == '\a' && buffer[bytesRead-2] == '\b') {
			break;
		}
		//osetreni kdyby prichazela rozkouzkovana zprava a jeden kousek byl delsi nez je max povolena delka te zpravy
		if (bytesRead >=maxLength && fullBufferLength >= maxLength && (fullMsgBuffer[fullBufferLength-2] != '\a' || fullMsgBuffer[fullBufferLength-1] != '\b')) {
			sendServerResponse(c, ServerMSG::SERVER_SYNTAX_ERROR);
			close(c);
			return true;
		}
	}
	int startOfMessage = 0;
	if (fullBufferLength >= 12) {
		string checkRechargingMessage = "";
		for (int i = 0 ; i < 12 ;i++) {
			checkRechargingMessage += fullMsgBuffer[i];
		}
		if (isRechargingResponseSyntaxOK(checkRechargingMessage)) {
			startOfMessage = 12;
			char fullPowerBuffer[BUFFER_SIZE];
			int fullPowerBufferLength = 0;
			tv.tv_sec = TIMEOUT_RECHARGING;
			select(c + 1, &sockets, NULL, NULL, &tv);
			if (receiveFullMessage(c, fullPowerBuffer, fullPowerBufferLength, 12, stashedMessages)) {
				return true;
			}
			tv.tv_sec = TIMEOUT;
			string fullPowerMsg = fullPowerBuffer;
			if (fullPowerMsg.compare(string{"FULL POWER\a\b"})) {
				sendServerResponse(c, ServerMSG::SERVER_LOGIC_ERROR);
				close(c);
				return true;
			}

			if (fullBufferLength == 12) {
				return receiveFullMessage(c, fullMsgBuffer, fullBufferLength, maxLength, stashedMessages);
			}
		}
	}

	//nejdriv odstrihnu prvni zpravu a zkontroluju jeji delku
	int whereToPut0 = startOfMessage + 1;
	for (int i = startOfMessage ; i < fullBufferLength ; i++) {
		if (i + 1 >= maxLength) {
			sendServerResponse(c, ServerMSG::SERVER_SYNTAX_ERROR);
			close(c);
			return true;
		}
		if (i+1 < fullBufferLength && fullMsgBuffer[i] == '\a' && fullMsgBuffer[i+1] == '\b') {
			whereToPut0 = i + 2;
			break;
		}
		if (maxLength == 7) {
			if (fullMsgBuffer[i] > 57 || fullMsgBuffer[i] < 48) {
				sendServerResponse(c, ServerMSG::SERVER_SYNTAX_ERROR);
				close(c);
				return true;	
			}
		}
	}

	char lastChar = '0';
	string message = "";
	for (int i = whereToPut0 ; i < fullBufferLength ; i++) {
		if (i == fullBufferLength - 1 && fullMsgBuffer[i] == '\0') {
			string msg = "\0";
			stashedMessages.push(message);
			break;
		}
		if (lastChar == '\a' && fullMsgBuffer[i] == '\b') {
			message += fullMsgBuffer[i];
			lastChar = '0';
			stashedMessages.push(message);
			message = "";
			continue;
		}
		lastChar = fullMsgBuffer[i];
		message += fullMsgBuffer[i];
	}

	fullMsgBuffer[whereToPut0] = '\0';
	fullBufferLength = whereToPut0;

	//pokud to je recharge
	string rechargingMessage = fullMsgBuffer;
	if (fullBufferLength == 12 && isRechargingResponseSyntaxOK(rechargingMessage)) {
		char fullPowerBuffer[BUFFER_SIZE];
		int fullPowerBufferLength = 0;
		tv.tv_sec = TIMEOUT_RECHARGING;
		select(c + 1, &sockets, NULL, NULL, &tv);
		if (receiveFullMessage(c, fullPowerBuffer, fullPowerBufferLength, 12, stashedMessages)) {
			return true;
		}
		tv.tv_sec = TIMEOUT;
		string fullPowerMsg = fullPowerBuffer;
		if (fullPowerMsg.compare(string{"FULL POWER\a\b"})) {
			sendServerResponse(c, ServerMSG::SERVER_LOGIC_ERROR);
			close(c);
			return true;
		}
		return receiveFullMessage(c, fullMsgBuffer, fullBufferLength, maxLength, stashedMessages);
	}
	return false;
}

//collects serverOk message after robot turn
bool collectServerOKMessage(int & c, queue<string> & stashedMessages) {
	char fullMsgBuffer[BUFFER_SIZE];
	int fullBufferLength = 0;
	if (receiveFullMessage(c, fullMsgBuffer, fullBufferLength, 12, stashedMessages)) {
		return true;
	}
	string okMsg = fullMsgBuffer;
	return !isMoveResponseSyntaxOK(okMsg);
}

bool sendServerTurnRight(int & c, queue<string> & stashedMessages) {
    string turnRightCommand("104 TURN RIGHT\a\b");
    if (send(c, turnRightCommand.c_str(), turnRightCommand.size(), 0) < 0) {
    	perror("Nemohu odeslat turn RIGHT command!");
		close(c);
		return true;
    }
    return collectServerOKMessage(c, stashedMessages);
}

void adjustDirectionAfterRightTurn(Direction & dir) {
	if (dir == Direction::UP)
		dir = Direction::RIGHT;
	else if (dir == Direction::RIGHT)
		dir = Direction::DOWN;
	else if (dir == Direction::DOWN)
		dir = Direction::LEFT;
	else
		dir = Direction::UP;
}

bool turnRobotToFaceGivenDirection(int & c, Direction & dir, const Direction & targetDir, queue<string> & stashedMessages) {
	while (dir != targetDir) {
		if (sendServerTurnRight(c, stashedMessages))
			return true;
		adjustDirectionAfterRightTurn(dir);
	}
	return false;
}

//converts moveMsg to stringStream and extracts number
void fillCoordinates(int & x, int & y, string & moveMsg) {
	stringstream ss;
	ss << moveMsg;
  
	string temp;
	int found;
	bool xFound = false; 
	while (!ss.eof()) { 
		ss >> temp; 
		if (stringstream(temp) >> found) {
			if (!xFound) {
				x = found;
				xFound = true;
			}
			else
				y = found;
		}
		temp = ""; 
	}
}
//also checks the move msg syntax + reacts to battery recharging
bool receiveMoveResponse(int & c, string & moveMsg, queue<string> & stashedMessages) {
    moveMsg = "";
    char fullMsgBuffer[BUFFER_SIZE];
    int bufferMsgLength = 0;
    if (receiveFullMessage(c, fullMsgBuffer, bufferMsgLength, 12, stashedMessages)) {
		return -3;
	}

	moveMsg = fullMsgBuffer;
	if (!isMoveResponseSyntaxOK(moveMsg)) {
    	sendServerResponse(c, ServerMSG::SERVER_SYNTAX_ERROR);
    	close(c);
    	return true;
	}
    return false;
}

bool sendAndReceiveMove(int & c, string & moveMsg, queue<string> & stashedMessages) {
	if (sendServerResponse(c, ServerMSG::SERVER_MOVE)) {
		return true;
	}
    if (receiveMoveResponse(c, moveMsg, stashedMessages)) {
    	return true;
    }
    return false;
}
//jde do cilove lokace a na kazdem poli se pokusi vyzvednout zpravu
bool moveToTargetPositionsAndTryToCollectMessage(int & c, const int targetX, const int targetY, Direction & dir, string & message, int & currentX, int & currentY, queue<string> & stashedMessages) {
	string moveMsg;
	int lastX, lastY;
	char fullMsgBuffer[BUFFER_SIZE];
	int fullBufferLength = 0;
	//pokud po ose X
	while (currentX != targetX) {
		if (currentX > targetX && dir != Direction::LEFT) {
			if (turnRobotToFaceGivenDirection(c, dir, Direction::LEFT, stashedMessages))
				return true;
			continue;
		} else if (currentX < targetX && dir != Direction::RIGHT) {
			if (turnRobotToFaceGivenDirection(c, dir, Direction::RIGHT, stashedMessages))
				return true;
			continue;
		}

		if (sendAndReceiveMove(c, moveMsg, stashedMessages)) {
			return true;
		}

		lastX = currentX;
		lastY = currentY;
		fillCoordinates(currentX, currentY, moveMsg);
		if (currentX == lastX && currentY == lastY) {
			continue;
		}
		//collect Message
		if (sendServerResponse(c, ServerMSG::SERVER_PICK_UP))
			return true;
		if (receiveFullMessage(c, fullMsgBuffer, fullBufferLength, 100, stashedMessages))
			return true;
		message = fullMsgBuffer;
		if (message.compare(string{"\a\b"})) {
			return false;
		}
	}
	//pohyb po ose Y
	while (currentY != targetY) {
		if (currentY > targetY && dir != Direction::DOWN) {
			if (turnRobotToFaceGivenDirection(c, dir, Direction::DOWN, stashedMessages))
				return true;
			continue;
		} else if (currentY < targetY && dir != Direction::UP) {
			if (turnRobotToFaceGivenDirection(c, dir, Direction::UP, stashedMessages))
				return true;
			continue;
		}

		if (sendAndReceiveMove(c, moveMsg, stashedMessages)) {
			return true;
		}

		lastX = currentX;
		lastY = currentY;
		fillCoordinates(currentX, currentY, moveMsg);
		if (currentX == lastX && currentY == lastY) {
			continue;
		}
		//collect Message
		if (sendServerResponse(c, ServerMSG::SERVER_PICK_UP))
			return true;
		if (receiveFullMessage(c, fullMsgBuffer, fullBufferLength, 100, stashedMessages))
			return true;
		message = fullMsgBuffer;
		if (message.compare(string{"\a\b"})) {
			return false;
		}
	}

	return false;
}

int main(int argc, char **argv) {
	//kontrola, ze jsou zadany alespon 2 parametry
	if(argc < 2){
		cerr << "Usage: server port" << endl;
		return -1;
	}

	int l = socket(AF_INET, SOCK_STREAM, 0);
	if(l < 0){
		perror("Nemohu vytvorit socket!");
		return -1;
	}

	int port = atoi(argv[1]);
	if(port == 0){
		cerr << "Usage: server port" << endl;
		close(l);
		return -1;
	}

	struct sockaddr_in adresa;
	bzero(&adresa, sizeof(adresa));
	adresa.sin_family = AF_INET;
	adresa.sin_port = htons(port);
	adresa.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(l, (struct sockaddr *) &adresa, sizeof(adresa)) < 0){
		perror("Problem s bind()!");
		close(l);
		return -1;
	}

	if(listen(l, 10) < 0){
		perror("Problem s listen()!");
		close(l);
		return -1;
	}

	struct sockaddr_in vzdalena_adresa;
	socklen_t velikost;

	while(true){
		int c = accept(l, (struct sockaddr *) &vzdalena_adresa, &velikost);
		if(c < 0){
			perror("Problem s accept()!");
			close(l);
			return -1;
		}
		pid_t pid = fork();
		if(pid == 0){
			close(l);

			struct timeval timeout;
			timeout.tv_sec = TIMEOUT;
			timeout.tv_usec = 0;
			int retval;
			char buffer[BUFFER_SIZE];
			char fullMsgBuffer[BUFFER_SIZE];
            queue<string> stashedMessages;
            int usernameHash = 0;
			tv.tv_sec = TIMEOUT;
			tv.tv_usec = 0;
			setsockopt(c, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
            Direction dir;
            while(true){
				FD_ZERO(&sockets);
				FD_SET(c, &sockets);
				retval = select(c + 1, &sockets, NULL, NULL, &timeout);
				if(retval < 0){
					perror("Chyba v select()");
					close(c);
					return -1;
				}
				if(!FD_ISSET(c, &sockets)){
					cout << "Connection timeout!" << endl;
					close(c);
					return 0;
				}
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//accepting name
				int bufferMsgLength = 0;
				if (receiveFullMessage(c, fullMsgBuffer, bufferMsgLength, 12, stashedMessages)) {
					return -3;
				}			
				int hash = 0;
                char lastChar = '0';
                for (int i = 0 ; i < bufferMsgLength ; i++) {
                    if (lastChar == '\a' && fullMsgBuffer[i] == '\b') {
                    	break;
                    }
                    if (fullMsgBuffer[i] == '\a') {
                    	if (lastChar == '\a') {
                    		hash += '\a';
                    	}
                    	lastChar = '\a';
                    	continue;
                    }
                    if (lastChar == '\a' && fullMsgBuffer[i] != '\b')
                    	hash += '\a';
                    hash += fullMsgBuffer[i];
                    lastChar = fullMsgBuffer[i];
                }
                hash *= 1000;
                hash = hash%65536;
                usernameHash = hash;
                //prictu klic serveru
                hash += 54621;
                hash = hash%65536;
                string responseForName;
                responseForName += std::to_string(hash);
                responseForName += "\a\b";
                if(send(c, responseForName.c_str(), responseForName.size(), 0) < 0){
                    perror("Nemohu odeslat data!");
                    close(c);
                    return -3;
                }
                cout << "odeslana odpoved: " << responseForName << " na jmeno: " << buffer << endl;
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//finish name confirmation
				if (receiveFullMessage(c, fullMsgBuffer, bufferMsgLength, 7, stashedMessages)) {
					return -3;
				}
				string received = fullMsgBuffer;
				string orizlyReceived = "";
				lastChar = '0';
				for (char& c : received) {
                   	if (lastChar == '\a' && c == '\b') {
                       	break;
                   	}
                   	if (c == '\a') {
                       	//todo tady rovnou break muze byt
                       	lastChar = c;
                       	continue;
                   	}
                   	orizlyReceived += c;
                   	lastChar = c;
                }
                const int receivedHash = atoi(orizlyReceived.c_str());
                const int expectedHash = (usernameHash + 45328)%65536;
                //pokud hash nesedi
                if (receivedHash != expectedHash) {
                    cout << "Vracim server login failed" << endl;
                	if (sendServerResponse(c, ServerMSG::SERVER_LOGIN_FAILED))
                		return -3;
                    close(c);
                    break;
                }
                if (sendServerResponse(c, ServerMSG::SERVER_OK))
                	return -3;
                cout << "Jmeno uspesne potvrzeno" << endl;
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//ted poslu move a nasmeruju ho do [2,2]
                string moveMsg = "";
                if (sendAndReceiveMove(c, moveMsg, stashedMessages))
                	return -3;
                cout << "dostal sem zpatky: " << moveMsg << endl;

                int lastPositionX;
                int lastPositionY;
                fillCoordinates(lastPositionX, lastPositionY, moveMsg);

                int currentPositionX = lastPositionX;
                int currentPositionY = lastPositionY;
                //potrebuji ziskat druhy move abych urcil smer
                while (currentPositionY == lastPositionY && currentPositionX == lastPositionX) {
                	if (sendAndReceiveMove(c, moveMsg, stashedMessages))
                		return -3;
                	fillCoordinates(currentPositionX, currentPositionY, moveMsg);
                }
                //urcim kam je namireny
                if (lastPositionX == currentPositionX) {
                    if (lastPositionY < currentPositionY) {
                        cout << "cumi nahoru" << endl;
                        dir = Direction::UP;
                    } else {
                        cout << "cumi dolu" << endl;
                        dir = Direction::DOWN;
                    }
                } else if (lastPositionY == currentPositionY) {
                    if (lastPositionX < currentPositionX) {
                        cout << "cumi vpravo" << endl;
                        dir = Direction::RIGHT;
                    } else {
                        cout << "cumi vlevo" << endl;
                        dir = Direction::LEFT;
                    }
                }

                //poslu ho do pravyho horniho rohu vzdy [2;2]
                while (currentPositionX != 2) {
					if (currentPositionX > 2 && dir != Direction::LEFT) {
						if (turnRobotToFaceGivenDirection(c, dir, Direction::LEFT, stashedMessages))
							return -3;
						continue;
					} else if (currentPositionX < 2 && dir != Direction::RIGHT) {
						if (turnRobotToFaceGivenDirection(c, dir, Direction::RIGHT, stashedMessages))
							return -3;
						continue;
					}
					if (sendAndReceiveMove(c, moveMsg, stashedMessages))
                		return -3;
					lastPositionX = currentPositionX;
					lastPositionY = currentPositionY;
					fillCoordinates(currentPositionX, currentPositionY, moveMsg);
                }
                while (currentPositionY != 2) {
                	if (currentPositionY > 2 && dir != Direction::DOWN) {
                		if (turnRobotToFaceGivenDirection(c, dir, Direction::DOWN, stashedMessages))
                			return -3;
                		continue;
                	} else if (currentPositionY < 2 && dir != Direction::UP) {
                		if (turnRobotToFaceGivenDirection(c, dir, Direction::UP, stashedMessages))
                			return -3;
                		continue;
                	}
					if (sendAndReceiveMove(c, moveMsg, stashedMessages))
                		return -3;
					lastPositionX = currentPositionX;
					lastPositionY = currentPositionY;
					fillCoordinates(currentPositionX, currentPositionY, moveMsg);
                }

                cout << "dostal sem se z obou whilu mam pozice" << currentPositionX << "--" << currentPositionY << endl;
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//collect message
                //vychazim z toho, ze jsem na pozici [2;2]
                string message;
				cout << "jdu vyzvednout zpravu!" << endl;
                //nejdriv ji zkusim collectnout jestli nahodou neni na 2,2
                if (sendServerResponse(c, ServerMSG::SERVER_PICK_UP))
					return true;
				if (receiveFullMessage(c, fullMsgBuffer, bufferMsgLength, 100, stashedMessages)) {
					return -3;
				}
				message = fullMsgBuffer;
				if (message.compare(string{"\a\b"})) {
					if (sendServerResponse(c, ServerMSG::SERVER_LOGOUT))
						return -3;
					break;
				}
				if (moveToTargetPositionsAndTryToCollectMessage(c, 2, -2, dir, message, currentPositionX, currentPositionY, stashedMessages))
					return -3;
				if (message.compare(string{"\a\b"})) {
					if (sendServerResponse(c, ServerMSG::SERVER_LOGOUT))
						return -3;
					break;
				}
				if (moveToTargetPositionsAndTryToCollectMessage(c, 1, 2, dir, message, currentPositionX, currentPositionY, stashedMessages))
					return -3;
				if (message.compare(string{"\a\b"})) {
					if (sendServerResponse(c, ServerMSG::SERVER_LOGOUT))
						return -3;
					break;
				}
				if (moveToTargetPositionsAndTryToCollectMessage(c, 0, -2, dir, message, currentPositionX, currentPositionY, stashedMessages))
					return -3;
				if (message.compare(string{"\a\b"})) {
					if (sendServerResponse(c, ServerMSG::SERVER_LOGOUT))
						return -3;
					break;
				}
				if (moveToTargetPositionsAndTryToCollectMessage(c, -1, 2, dir, message, currentPositionX, currentPositionY, stashedMessages))
					return -3;
				if (message.compare(string{"\a\b"})) {
					if (sendServerResponse(c, ServerMSG::SERVER_LOGOUT))
						return -3;
					break;
				}
				if (moveToTargetPositionsAndTryToCollectMessage(c, -2, -2, dir, message, currentPositionX, currentPositionY, stashedMessages))
					return -3;
				if (message.compare(string{"\a\b"})) {
					if (sendServerResponse(c, ServerMSG::SERVER_LOGOUT))
						return -3;
					break;
				}
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
				break;
			}
			close(c);
			return 0;
		}
		close(c);
	}

	close(l);
	return 0;
}
