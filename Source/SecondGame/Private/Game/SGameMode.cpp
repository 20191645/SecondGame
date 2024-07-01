// SGameMode.cpp

#include "Game/SGameMode.h"
#include "Controller/SPlayerController.h"
#include "Game/SPlayerState.h"

ASGameMode::ASGameMode()
{
	// �÷��̾� ��Ʈ�ѷ� ����
	// ��Ƽ�÷��� ��� -- Class ������ �÷��̾� ���帶�� Instance�� ������
	PlayerControllerClass = ASPlayerController::StaticClass();
}

void ASGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// ���� ������ �÷��̾� ĳ������ 'PlayerState' �ʱ�ȭ
	ASPlayerState* PlayerState = NewPlayer->GetPlayerState<ASPlayerState>();
	if (true == ::IsValid(PlayerState))
	{
		PlayerState->InitPlayerState();
	}
}
