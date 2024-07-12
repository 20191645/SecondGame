// SPlayerController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

class USHUD;
class USGameResultWidget_Single;

UCLASS()
class SECONDGAME_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// 'HUDWidget' Getter �Լ�
	USHUD* GetHUDWidget() const { return HUDWidget; };

	// ���۹� â ��� �Լ�
	void ToggleManualWidget();

protected:
	virtual void BeginPlay() override;

	// 'OnCurrentKillCountReachedMaxDelegate' ��������Ʈ�� ���ε��� �Լ�
	UFUNCTION()
	void OnCurrentKillCountReachedMax();
	// 'OnCurrentDeathCountReachedMaxDelegate' ��������Ʈ�� ���ε��� �Լ�
	UFUNCTION()
	void OnCurrentDeathCountReachedMax();

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

	// ���� Ŭ���� ���� Ŭ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<USGameResultWidget_Single> GameClearWidgetClass;
	// ���� ���� ���� Ŭ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<USGameResultWidget_Single> GameOverWidgetClass;

	// ���۹� ���� Ŭ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> ManualWidgetClass;
	// ���۹� ���� Ŭ���� ��ü
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UUserWidget> ManualWidgetInstance;
	// ���۹� ���� ��� Ȯ��
	bool bIsManualOn = false;
};
