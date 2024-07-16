// SGameMode_Multi.cpp

#include "Game/SGameMode_Multi.h"
#include "Controller/SPlayerController_Multi.h"
#include "Game/SPlayerState.h"
#include "Game/SGameState.h"

ASGameMode_Multi::ASGameMode_Multi()
{
	// 플레이어 컨트롤러 설정
	// 멀티플레이 고려 -- Class 정보로 플레이어 입장마다 Instance를 생성함
	PlayerControllerClass = ASPlayerController_Multi::StaticClass();
}

void ASGameMode_Multi::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// 현재 게임 상태가 플레이어 대기중이 아니면 입장 막기
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

void ASGameMode_Multi::BeginPlay()
{
	Super::BeginPlay();

	// 1초마다 'OnMainTimerElapsed()' 함수 호출
	GetWorld()->GetTimerManager().SetTimer(
		MainTimerHandle, this, &ThisClass::OnMainTimerElapsed, true, 1.0f);

	// 'RemainWaitingTimeForPlaying' 초기화
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

		// 게임 시작 최소 인원을 못 채웠을 경우
		if (AlivePlayerControllers.Num() < MinimumPlayerCountForPlaying)
		{
			NotificationString = FString::Printf(TEXT("Wait another players..."));
			// 남은 대기 시간 초기화
			RemainWaitingTimeForPlaying = WaitingTime;
		}
		// 게임 시작 최소 인원을 채웠을 경우
		else
		{
			NotificationString = FString::Printf(TEXT("Wait %d seconds for playing"), RemainWaitingTimeForPlaying);
			// 대기 시간 감소
			RemainWaitingTimeForPlaying--;
		}

		// 게임 시작
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
			// 살아남은 플레이어 수 체크
			SGameState->AlivePlayerControllerCount = AlivePlayerControllers.Num();

			FString NotificationString = 
				FString::Printf(TEXT("Players: %d / %d"), 
					SGameState->AlivePlayerControllerCount, SGameState->AlivePlayerControllerCount + DeadPlayerControllers.Num());

			NotifyToAllPlayer(NotificationString);

			// 살아남은 플레이어가 1명일 경우 게임 종료
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
