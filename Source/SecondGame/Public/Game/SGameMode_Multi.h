// SGameMode_Multi.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode_Multi.generated.h"

class ASPlayerController_Multi;

UCLASS()
class SECONDGAME_API ASGameMode_Multi : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASGameMode_Multi();

	// 플레이어가 게임에 입장한 후 호출될 함수
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// 플레이어가 게임에서 나간 후 호출될 함수
	virtual void Logout(AController* Exiting) override;

	virtual void BeginPlay() override;

	// 플레이어 탈락 시 호출
	void OnControllerDead(ASPlayerController_Multi* InDeadController);

	// 'AlivePlayerControllers' Getter 함수
	TArray<ASPlayerController_Multi*> GetAlivePlayerControllers() { return AlivePlayerControllers; }

private:
	// 'MainTimerHandle' 타이머에 반응할 함수
	UFUNCTION()
	void OnMainTimerElapsed();

	// 게임에 입장한 모든 플레이어에게 알림 메시지 전달
	void NotifyToAllPlayer(const FString& NotificationString);

public:
	FTimerHandle MainTimerHandle;

	// 기본 게임 입장 대기 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASGameMode_Multi")
	int32 WaitingTime = 7;
	// 앞으로 게임에 입장하기까지 남은 시간
	int32 RemainWaitingTimeForPlaying = 7;
	// 게임 시작 최소 인원
	int32 MinimumPlayerCountForPlaying = 2;

	// 기본 게임 종료 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASGameMode_Multi")
	int32 EndingTime = 10;
	// 게임 종료까지 남은 시간
	int32 RemainWaitingTimeForEnding = 10;

protected:
	// 게임에 남아있는 플레이어
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASGameMode_Multi", Meta = (AllowPrivateAccess))
	TArray<TObjectPtr<ASPlayerController_Multi>> AlivePlayerControllers;
	// 게임에서 탈락한 플레이어
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASGameMode_Multi", Meta = (AllowPrivateAccess))
	TArray<TObjectPtr<ASPlayerController_Multi>> DeadPlayerControllers;
};
