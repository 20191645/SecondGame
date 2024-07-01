// SGameMode.cpp

#include "Game/SGameMode.h"
#include "Controller/SPlayerController.h"
#include "Game/SPlayerState.h"

ASGameMode::ASGameMode()
{
	// 플레이어 컨트롤러 설정
	// 멀티플레이 고려 -- Class 정보로 플레이어 입장마다 Instance를 생성함
	PlayerControllerClass = ASPlayerController::StaticClass();
}

void ASGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// 새로 입장한 플레이어 캐릭터의 'PlayerState' 초기화
	ASPlayerState* PlayerState = NewPlayer->GetPlayerState<ASPlayerState>();
	if (true == ::IsValid(PlayerState))
	{
		PlayerState->InitPlayerState();
	}
}
