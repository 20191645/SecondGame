// SGameMode_Multi.cpp

#include "Game/SGameMode_Multi.h"
#include "Controller/SPlayerController_Multi.h"
#include "Game/SPlayerState.h"

ASGameMode_Multi::ASGameMode_Multi()
{
	// 플레이어 컨트롤러 설정
	// 멀티플레이 고려 -- Class 정보로 플레이어 입장마다 Instance를 생성함
	PlayerControllerClass = ASPlayerController_Multi::StaticClass();
}

void ASGameMode_Multi::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// 새로 입장한 플레이어 캐릭터의 'PlayerState' 초기화
	ASPlayerState* PlayerState = NewPlayer->GetPlayerState<ASPlayerState>();
	if (true == ::IsValid(PlayerState))
	{
		PlayerState->InitPlayerState();
	}

	// 'AlivePlayerControllers'에 새로 입장한 플레이어 캐릭터 정보 추가
	ASPlayerController_Multi* NewPlayerController = Cast<ASPlayerController_Multi>(NewPlayer);
	if (true == ::IsValid(NewPlayerController))
	{
		AlivePlayerControllers.Add(NewPlayerController);
		// 알림창 위젯 텍스트 수정
		NewPlayerController->NotificationText = FText::FromString(TEXT("Welcome to SecondGame"));
	}
}

void ASGameMode_Multi::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	// 게임에서 나간 플레이어 캐릭터 정보 추가
	ASPlayerController_Multi* ExitingPlayerController = Cast<ASPlayerController_Multi>(Exiting);
	if (true == ::IsValid(ExitingPlayerController) && 
		INDEX_NONE != AlivePlayerControllers.Find(ExitingPlayerController))
	{
		// 'AlivePlayerControllers'에 게임에서 나간 플레이어 캐릭터 정보 삭제
		AlivePlayerControllers.Remove(ExitingPlayerController);
		// 'DeadPlayerControllers'에 게임에서 나간 플레이어 캐릭터 정보 추가
		DeadPlayerControllers.Add(ExitingPlayerController);
	}
}
