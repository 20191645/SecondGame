// SBW_Bar.h

#pragma once

#include "CoreMinimal.h"
#include "UI/SBaseWidget.h"
#include "SBW_Bar.generated.h"

class UProgressBar;

UCLASS()
class SECONDGAME_API USBW_Bar : public USBaseWidget
{
	GENERATED_BODY()
	
public:
	// Widget Ŭ������ ������
	USBW_Bar(const FObjectInitializer& ObjectInitializer);

	// 'MaxFigure' Setter �Լ�
	void SetMaxFigure(float InMaxFigure);

protected:
	// �� ����
	// -- BindWidget: �ش� �Ӽ��� ���� �̸��� ������ ���ε�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USBW_Bar", meta = (BindWidget))
	TObjectPtr<UProgressBar> BarWidget;

	// 'BarWidget' ��ġ �ִ밪
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USBW_Bar")
	float MaxFigure;
};
