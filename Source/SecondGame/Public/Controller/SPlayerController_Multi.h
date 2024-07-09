// SPlayerController_Multi.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController_Multi.generated.h"

class USHUD;

UCLASS()
class SECONDGAME_API ASPlayerController_Multi : public APlayerController
{
	GENERATED_BODY()

public:
	// 'HUDWidget' Getter 함수
	USHUD* GetHUDWidget() const { return HUDWidget; };

protected:
	virtual void BeginPlay() override;

private:
	// 조준점 위젯 클래스 정보
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> CrosshairUIClass;

	// HUD 위젯 클래스 객체
	UPROPERTY();
	TObjectPtr<USHUD> HUDWidget;
	// HUD 위젯 클래스 정보
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess));
	TSubclassOf<USHUD> HUDWidgetClass;
	
};
