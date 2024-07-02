// SPlayerController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

class USHUD;

UCLASS()
class SECONDGAME_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// 'HUDWidget' Getter �Լ�
	USHUD* GetHUDWidget() const { return HUDWidget; };

protected:
	virtual void BeginPlay() override;

private:
	// ������ ���� Ŭ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ASPlayerController, Meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> CrosshairUIClass;

	// HUD ���� Ŭ���� ��ü
	UPROPERTY();
	TObjectPtr<USHUD> HUDWidget;
	// HUD ���� Ŭ���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess));
	TSubclassOf<USHUD> HUDWidgetClass;
};
