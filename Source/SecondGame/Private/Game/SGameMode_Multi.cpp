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
}
