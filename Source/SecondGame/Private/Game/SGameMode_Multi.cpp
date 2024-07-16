// SGameMode_Multi.cpp

#include "Game/SGameMode_Multi.h"
#include "Controller/SPlayerController_Multi.h"
#include "Game/SPlayerState.h"
#include "Game/SGameState.h"

ASGameMode_Multi::ASGameMode_Multi()
{
	// �÷��̾� ��Ʈ�ѷ� ����
	// ��Ƽ�÷��� ��� -- Class ������ �÷��̾� ���帶�� Instance�� ������
	PlayerControllerClass = ASPlayerController_Multi::StaticClass();
}

void ASGameMode_Multi::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// ���� ���� ���°� �÷��̾� ������� �ƴϸ� ���� ����
	ASGameState* SGameState = GetGameState<ASGameState>();
	if (false == SGameState)
	{
		return;
	}
	if (SGameState->MatchState != EMatchState::Waiting)
	{
		NewPlayer->SetLifeSpan(0.1f);
		return;
	}

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

void ASGameMode_Multi::BeginPlay()
{
	Super::BeginPlay();

	// 1�ʸ��� 'OnMainTimerElapsed()' �Լ� ȣ��
	GetWorld()->GetTimerManager().SetTimer(
		MainTimerHandle, this, &ThisClass::OnMainTimerElapsed, true, 1.0f);

	// 'RemainWaitingTimeForPlaying' �ʱ�ȭ
	RemainWaitingTimeForPlaying = WaitingTime;
}

void ASGameMode_Multi::OnControllerDead(ASPlayerController_Multi* InDeadController)
{
	if (IsValid(InDeadController) == false 
		|| AlivePlayerControllers.Find(InDeadController) == INDEX_NONE)
	{
		return;
	}

	AlivePlayerControllers.Remove(InDeadController);
	DeadPlayerControllers.Add(InDeadController);
}

void ASGameMode_Multi::OnMainTimerElapsed()
{
	ASGameState* SGameState = GetGameState<ASGameState>();
	if (false == ::IsValid(SGameState))
	{
		return;
	}

	switch (SGameState->MatchState)
	{
	case EMatchState::None:
		break;
	case EMatchState::Waiting:
	{
		FString NotificationString = FString::Printf(TEXT(""));

		// ���� ���� �ּ� �ο��� �� ä���� ���
		if (AlivePlayerControllers.Num() < MinimumPlayerCountForPlaying)
		{
			NotificationString = FString::Printf(TEXT("Wait another players..."));
			// ���� ��� �ð� �ʱ�ȭ
			RemainWaitingTimeForPlaying = WaitingTime;
		}
		// ���� ���� �ּ� �ο��� ä���� ���
		else
		{
			NotificationString = FString::Printf(TEXT("Wait %d seconds for playing"), RemainWaitingTimeForPlaying);
			// ��� �ð� ����
			RemainWaitingTimeForPlaying--;
		}

		// ���� ����
		if (0 == RemainWaitingTimeForPlaying)
		{
			NotificationString = FString::Printf(TEXT("Let's Game Start!"));
			SGameState->MatchState = EMatchState::Playing;
		}

		NotifyToAllPlayer(NotificationString);

		break;
	}
	case EMatchState::Playing:
	{
		if (IsValid(SGameState) == true)
		{
			// ��Ƴ��� �÷��̾� �� üũ
			SGameState->AlivePlayerControllerCount = AlivePlayerControllers.Num();

			FString NotificationString = 
				FString::Printf(TEXT("Players: %d / %d"), 
					SGameState->AlivePlayerControllerCount, SGameState->AlivePlayerControllerCount + DeadPlayerControllers.Num());

			NotifyToAllPlayer(NotificationString);

			// ��Ƴ��� �÷��̾ 1���� ��� ���� ����
			if (SGameState->AlivePlayerControllerCount <= 1)
			{
				SGameState->MatchState = EMatchState::Ending;
			}
		}

		break;
	}
	case EMatchState::Ending:
		break;
	case EMatchState::End:
		break;
	default:
		break;
	}
}

void ASGameMode_Multi::NotifyToAllPlayer(const FString& NotificationString)
{
	for (auto AlivePlayerController : AlivePlayerControllers)
	{
		AlivePlayerController->NotificationText = FText::FromString(NotificationString);
	}

	for (auto DeadPlayerController : DeadPlayerControllers)
	{
		DeadPlayerController->NotificationText = FText::FromString(NotificationString);
	}
}
