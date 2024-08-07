// SGameMode_Multi.cpp

#include "Game/SGameMode_Multi.h"
#include "Controller/SPlayerController_Multi.h"
#include "Game/SPlayerState.h"
#include "Game/SGameState.h"
#include "Kismet/GameplayStatics.h"

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
		PlayerState->InitPlayerState_Client();
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

	// 'RemainWaitingTimeForEnding' �ʱ�ȭ
	RemainWaitingTimeForEnding = EndingTime;
}

void ASGameMode_Multi::OnControllerDead(ASPlayerController_Multi* InDeadController)
{
	if (IsValid(InDeadController) == false 
		|| AlivePlayerControllers.Find(InDeadController) == INDEX_NONE)
	{
		return;
	}
	
	ASPlayerState* SPlayerState = InDeadController->GetPlayerState<ASPlayerState>();
	if (IsValid(SPlayerState) == true)
	{
		// ���� �й� ���� ȭ�鿡 �߰� -- �ش� ��Ʈ�ѷ��� �ִ� ���� �� ����
		if (SPlayerState->GetCurrentDeathCount() >= SPlayerState->GetMaxDeathCount()) {
			InDeadController->ShowLoserUI(AlivePlayerControllers.Num());
		}
		// ���� �й� ���� ȭ�鿡 �߰� -- �ٸ� ��Ʈ�ѷ��� �ִ� ų �� ����
		else {
			InDeadController->ShowLoserUI(2);
		}
	}

	AlivePlayerControllers.Remove(InDeadController);
	DeadPlayerControllers.Add(InDeadController);
}

void ASGameMode_Multi::NotifyKillLog(const FString& KillLogString)
{
	// ��� �÷��̾�� ų�α� �˸�
	NotifyToAllPlayer(KillLogString);
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
			NotificationString = FString::Printf(TEXT("Wait %d seconds for playing"), RemainWaitingTimeForPlaying - 1);
			// ��� �ð� ����
			RemainWaitingTimeForPlaying--;
		}

		// ���� ����
		if (0 == RemainWaitingTimeForPlaying)
		{
			NotificationString = FString::Printf(TEXT("Let's start Second Game!"));
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
				// ���� �¸� ���� ȭ�鿡 �߰�
				AlivePlayerControllers[0]->ShowWinnerUI();
			}
		}

		break;
	}
	case EMatchState::Ending:
	{
		--RemainWaitingTimeForEnding;

		if (RemainWaitingTimeForEnding <= 0)
		{
			for (auto AliveController : AlivePlayerControllers)
			{
				AliveController->ReturnToTitle();
			}
			for (auto DeadController : DeadPlayerControllers)
			{
				DeadController->ReturnToTitle();
			}

			// Ÿ�̸� Ŭ����
			MainTimerHandle.Invalidate();

			// ���� ���� �ʱ�ȭ
			FName CurrentLevelName = FName(UGameplayStatics::GetCurrentLevelName(this));
			UGameplayStatics::OpenLevel(this, CurrentLevelName, true, FString(TEXT("listen")));

			return;
		}

		break;
	}
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
