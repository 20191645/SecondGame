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
	// 'HUDWidget' Getter �Լ�
	USHUD* GetHUDWidget() const { return HUDWidget; };

	// ���۹� â ��� �Լ�
	void ToggleManualWidget();

protected:
	virtual void BeginPlay() override;

private:
	// ������ ���� Ŭ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> CrosshairUIClass;

	// HUD ���� Ŭ���� ��ü
	UPROPERTY();
	TObjectPtr<USHUD> HUDWidget;
	// HUD ���� Ŭ���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess));
	TSubclassOf<USHUD> HUDWidgetClass;
	
	// ���۹� ���� Ŭ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> ManualWidgetClass;
	// ���۹� ���� Ŭ���� ��ü
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UUserWidget> ManualWidgetInstance;
	// ���۹� ���� ��� Ȯ��
	bool bIsManualOn = false;
};
