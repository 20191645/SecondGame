// SGameMode_Multi.cpp

#include "Game/SGameMode_Multi.h"
#include "Controller/SPlayerController_Multi.h"
#include "Game/SPlayerState.h"

ASGameMode_Multi::ASGameMode_Multi()
{
	// �÷��̾� ��Ʈ�ѷ� ����
	// ��Ƽ�÷��� ��� -- Class ������ �÷��̾� ���帶�� Instance�� ������
	PlayerControllerClass = ASPlayerController_Multi::StaticClass();
}

void ASGameMode_Multi::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// ���� ������ �÷��̾� ĳ������ 'PlayerState' �ʱ�ȭ
	ASPlayerState* PlayerState = NewPlayer->GetPlayerState<ASPlayerState>();
	if (true == ::IsValid(PlayerState))
	{
		PlayerState->InitPlayerState();
	}

	// 'AlivePlayerControllers'�� ���� ������ �÷��̾� ĳ���� ���� �߰�
	ASPlayerController_Multi* NewPlayerController = Cast<ASPlayerController_Multi>(NewPlayer);
	if (true == ::IsValid(NewPlayerController))
	{
		AlivePlayerControllers.Add(NewPlayerController);
		// �˸�â ���� �ؽ�Ʈ ����
		NewPlayerController->NotificationText = FText::FromString(TEXT("Welcome to SecondGame"));
	}
}

void ASGameMode_Multi::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	// ���ӿ��� ���� �÷��̾� ĳ���� ���� �߰�
	ASPlayerController_Multi* ExitingPlayerController = Cast<ASPlayerController_Multi>(Exiting);
	if (true == ::IsValid(ExitingPlayerController) && 
		INDEX_NONE != AlivePlayerControllers.Find(ExitingPlayerController))
	{
		// 'AlivePlayerControllers'�� ���ӿ��� ���� �÷��̾� ĳ���� ���� ����
		AlivePlayerControllers.Remove(ExitingPlayerController);
		// 'DeadPlayerControllers'�� ���ӿ��� ���� �÷��̾� ĳ���� ���� �߰�
		DeadPlayerControllers.Add(ExitingPlayerController);
	}
}
