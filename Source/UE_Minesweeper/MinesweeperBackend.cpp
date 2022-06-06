#include "MinesweeperBackend.h"
#include "WebSocketsModule.h"

void UMinesweeperBackend::Connect()
{
	if(Initialized)
		return;
	
	const FString ServerURL = TEXT("wss://hometask.eg1236.com/game1/");
	const FString ServerProtocol = TEXT("wss");
    
	Socket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);

	Socket->OnConnected().AddLambda([]() -> void {
		UE_LOG(LogTemp, Warning, TEXT("Connected"));
	});
    
	Socket->OnConnectionError().AddLambda([](const FString & Error) -> void {
		UE_LOG(LogTemp, Warning, TEXT("Connection failed"));
	});
    
	Socket->OnClosed().AddLambda([](int32 StatusCode, const FString& Reason, bool bWasClean) -> void {
		UE_LOG(LogTemp, Warning, TEXT("Connection closed: %s"), *Reason);
	});
    
	Socket->OnMessage().AddLambda([this](const FString & Message) -> void {
		UE_LOG(LogTemp, Warning, TEXT("Recieved string: %s"), *Message);
		OnMessageRecieved(Message);
	});
    
	Socket->OnRawMessage().AddLambda([](const void* Data, SIZE_T Size, SIZE_T BytesRemaining) -> void {
		UE_LOG(LogTemp, Warning, TEXT("Recieved byte code"));
	});
    
	Socket->OnMessageSent().AddLambda([](const FString& MessageString) -> void {
		UE_LOG(LogTemp, Warning, TEXT("Message sent: %s"), *MessageString);
	});
    
	Socket->Connect();

	Initialized = true;
}

void UMinesweeperBackend::RequestNewLevel(int level)
{
	LastRequestedAction = MESSAGE_TYPE::NewLevel;
	Send("new " + FString::FromInt(level));
}

void UMinesweeperBackend::RequestMap()
{
	LastRequestedAction = MESSAGE_TYPE::Map;
	Send("map");
}

void UMinesweeperBackend::RequestOpenBlock(int x, int y)
{
	LastRequestedAction = MESSAGE_TYPE::Open;
	Send("open " + FString::FromInt(x) + " " + FString::FromInt(y));
}

void UMinesweeperBackend::Send(const FString msg)
{
	if(!Initialized)
		return;
	if (!Socket->IsConnected())
		return;
    
	Socket->Send(msg);
}

void UMinesweeperBackend::OnMessageRecieved(const FString& message)
{
	switch (LastRequestedAction)
	{
	case NewLevel:
		OnNewLevelRequestComplete.Broadcast(message);
		break;
	case Map:
		OnMapRequestComplete.Broadcast(message);
		break;
	case Open:
		OnOpenBlockRequestComplete.Broadcast(message);
		break;
	case None:
	default:
		break;
	}
}

void UMinesweeperBackend::Close()
{
	if(!Initialized)
		return;
	if (!Socket->IsConnected())
		return;
	
	Socket->Close();
	Initialized = false;
}
