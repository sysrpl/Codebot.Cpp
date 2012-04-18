#include <Codebot/Packages/System.h>
#include <Codebot/Packages/Expressions.h>
#include <Codebot/Packages/Net.h>
#include <Codebot/Packages/Threading.h>

using namespace Codebot;
using namespace Codebot::Collections;
using namespace Codebot::Expressions;
using namespace Codebot::IO;
using namespace Codebot::Net;
using namespace Codebot::Threading;

Integer connection = 1;

void Request(Thread* thread, Socket* client)
{
	auto request = New<WebRequest>();
	Socket* server = client->Server();
	while (server->Accept(client))
	{
		Integer i = connection++;
		WriteLine("Accepted request #{0}", i);
		request->Reset();
		while (!request->Complete())
		{
			String s;
			if (client->Read(s))
				request->Add(s);
			else
				break;
		}
		if (request->Complete())
		{
			String response;
			Integer code = request->BuildResponse(response);
			if (client->Write(response))
			{
				WriteLine("Response #{0} {1} {2}", i, request->Resource(), code);
				WriteLine("{0} bytes transferred", response.Length());
			}
			else
				WriteLine("Error sending response #{0}", i);
		}
		else
			WriteLine("Error receiving request #{0}", i);
	}
}

typedef Dictionary<Ref<Thread>, Ref<Socket>> Threads;

String webroot;
String address = "localhost";
Integer port = 8080;

void WebServer()
{
	Directory::Change(webroot);
	auto server = New<Socket>();
	server->Listen(address, port);
	if (server->State() != SocketStateServer)
	{
		WriteLine("Could not open port");
		return;
	}
	WriteLine("Web server listening on {0}:{1}", server->Address(), server->Port());
	auto threads = New<Threads>();
	Integer count = 10;
	for (Integer i = 0; i < count; i++)
	{
		auto client = New<Socket>(server);
		auto thread = New<Thread>((ThreadProc)Request, client);
		threads->Add(thread, client);
	}
	WriteLine("Press [return] to terminate");
	ReadLine();
	server->Close();
	ForEach(pair, threads)
	{
		pair->Value()->Close();
		pair->Key()->Wait();
	}
	WriteLine("Terminated");
}

int main()
{
	webroot = Path::Combine(".", "website");
	Application::FindSwitch("webroot:", webroot);
	Application::FindSwitch("address:", address);
	Application::FindSwitch("port:", port);
	return Application::Run(WebServer);
}
