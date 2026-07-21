#undef UNICODE
#include <iostream>
#include <ws2tcpip.h>
// C++ 20
#include <format>

using namespace std;

int main(int argc, char* argv[])
{
	std::cout << "Hello from WinAPI test console app!" << std::endl;

	if(argc < 3)
	{
		cout << "Example: main.exe www.google.com 443" << endl;
		return 1;
	}

	const std::string hostname = argv[1];
	const std::string port = argv[2];

	std::cout << "Host: " << hostname << std::endl;
	std::cout << "Port: " << port << std::endl;

	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // IPv4 and IPv6
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_CANONNAME;

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(iResult != 0)
	{
		std::cout << "WSAStartup failed" << std::endl;
		return 1;
	}

	struct addrinfo *result = NULL;
	DWORD dwRetVal = getaddrinfo(hostname.c_str(), port.c_str(), &hints, &result);
	if(dwRetVal != 0)
	{
		std::cout << "getaddrinfo failed with error: " << dwRetVal << std::endl;
		WSACleanup();
		return 1;
	}

	struct addrinfo *ptr = NULL;
	struct sockaddr_in *sockaddr_ipv4 = NULL;

	for(ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		std::cout << "\nAn IP address info will be here soon!" << std::endl;

		std::cout << "IP address Family:" << std::endl;
		switch(ptr->ai_family)
		{
			case AF_UNSPEC:
			{
				std::cout << "Unspecified" << std::endl;
				break;
			}
			case AF_INET:
			{
				std::cout << "Begin resolve IPv4 address" << std::endl;
				sockaddr_ipv4 = (struct sockaddr_in *)ptr->ai_addr;
				std::cout << "IPv4 address: " << inet_ntoa(sockaddr_ipv4->sin_addr) << std::endl;
				break;
			}
			case AF_INET6:
			{
				std::cout << "Begin resolve IPv6 address" << std::endl;

				LPSOCKADDR sockaddr_ip = (LPSOCKADDR)ptr->ai_addr;
				DWORD ipbufferlength = 46;
				char ipstringbuffer[46];
				INT iRetVal = WSAAddressToString(sockaddr_ip, (DWORD)ptr->ai_addrlen, NULL, ipstringbuffer, &ipbufferlength);
				if(iRetVal)
				{
					std::cout << "WSAAddressToString failed with error: " << WSAGetLastError();
				}
				else
				{
					std::cout << "Resolved IPv6 address: " << ipstringbuffer << std::endl;
				}

				break;
			}
			case AF_NETBIOS:
			{
				std::cout << "AF_NETBIOS (NetBIOS)" << std::endl;
				break;
			}	
			default:
			{
				std::cout << "Unhandled address type: " << ptr->ai_family << std::endl;
				break;
			}
		}

		std::cout << "Web Socket type:" << std::endl;
		switch(ptr->ai_socktype)
		{
		case 0:
			std::cout << "Unspecified" << std::endl;
			break;
		case SOCK_STREAM:
			cout << "SOCK_STREAM (stream)" << endl;
			break;
		case SOCK_DGRAM:
			cout << "SOCK_DGRAM (datagram socket)" << endl;
			break;
		case SOCK_RAW:
			cout << "SOCK_RAW" << endl;
			break;
		case SOCK_RDM:
			cout << "SOCK_RDM" << endl;
			break;
		case SOCK_SEQPACKET:
			cout << "SOCK_SEQPACKET" << endl;
			break;
		default:
			cout << "Other socket type: " << ptr->ai_socktype << endl;
			break;
		}

		std::cout << "Web Socket Protocol:" << std::endl;
		switch(ptr->ai_protocol)
		{
		case 0:
			cout << "Unspecified" << endl;
			break;
		case IPPROTO_TCP:
			cout << "IPPROTO_TCP (TCP)" << endl;
			break;
		case IPPROTO_UDP:
			cout << "IPPROTO_UDP (UDP)" << endl;
			break;
		default:
			cout << "Other protocol type: " << ptr->ai_protocol << endl;
			break;
		}

		cout << format("Length of this sockaddr: {}\n", ptr->ai_addrlen);

		cout << "Before canon name" << endl;
		if(ptr->ai_canonname)
		{
			const string canonName = ptr->ai_canonname;
			cout << "Canon name: " << ptr->ai_canonname << endl;
			// cout << format("Canonical name: {}\n", canonName;
		}
		else
		{
			cout << "AI Canon Name is not defined!\n";
		}
	}

	std::string userInput;
	std::cout << "Press any key to continue...";
	std::cin >> userInput;

	// Structure and pointers cleanup
	freeaddrinfo(result);
	// Session cleanup
	WSACleanup();

	return 0;
}