// SPlayerController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

UCLASS()
class SECONDGAME_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:
	// 조준점 위젯 클래스 정보
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ASPlayerController, Meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> CrosshairUIClass;
};
