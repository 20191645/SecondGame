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

protected:
	// 현재 살아있는 플레이어
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASGameMode_Multi", Meta = (AllowPrivateAccess))
	TArray<TObjectPtr<ASPlayerController_Multi>> AlivePlayerControllers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASGameMode_Multi", Meta = (AllowPrivateAccess))
	TArray<TObjectPtr<ASPlayerController_Multi>> DeadPlayerControllers;
};
