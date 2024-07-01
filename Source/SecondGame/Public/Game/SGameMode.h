// SGameMode.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

UCLASS()
class SECONDGAME_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASGameMode();

	// 플레이어가 게임에 입장한 후 호출될 함수
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
