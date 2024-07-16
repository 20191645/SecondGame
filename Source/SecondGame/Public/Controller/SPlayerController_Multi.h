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

	// 조작법 창 토글 함수
	void ToggleManualWidget();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 플레이어 죽으면 게임 모드에 전달
	void OnOwningCharacterDead();

protected:
	virtual void BeginPlay() override;

public:
	// 'NotificationWidget'에 적용될 텍스트
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FText NotificationText;

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
	
	// 조작법 위젯 클래스 정보
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> ManualWidgetClass;
	// 조작법 위젯 클래스 객체
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UUserWidget> ManualWidgetInstance;
	// 조작법 위젯 토글 확인
	bool bIsManualOn = false;

	// 알림창 위젯 클래스 정보
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> NotificationWidgetClass;
};
