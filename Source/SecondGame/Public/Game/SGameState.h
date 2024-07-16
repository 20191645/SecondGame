// SGameState.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SGameState.generated.h"

// 게임 진행 상태를 나타내는 enum 클래스
UENUM(BlueprintType)
enum class EMatchState : uint8
{
	None,
	Waiting,
	Playing,
	Ending,
	End
};

UCLASS()
class SECONDGAME_API ASGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// 현재 살아남은 플레이어 수
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "ASGameState")
	int32 AlivePlayerControllerCount;

	// 현재 게임 진행 상태
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "ASGameState")
	EMatchState MatchState = EMatchState::Waiting;
};
