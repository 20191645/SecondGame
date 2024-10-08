// SGameMode.cpp

#include "Game/SGameMode.h"
#include "Controller/SPlayerController.h"
#include "Game/SPlayerState.h"
#include "Character/SNonPlayerCharacter.h"

ASGameMode::ASGameMode()
{
	// 플레이어 컨트롤러 설정
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

	// 알림창 텍스트 초기화
	ASPlayerController* NewPlayerController = Cast<ASPlayerController>(NewPlayer);
	if (IsValid(NewPlayerController) == true) {
		NewPlayerController->NotificationText = FText::FromString(TEXT("Let's start Second Game!"));
	}
}