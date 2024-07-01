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
	// Widget 클래스의 생성자
	USBW_Bar(const FObjectInitializer& ObjectInitializer);

	// 'MaxFigure' Setter 함수
	void SetMaxFigure(float InMaxFigure);

protected:
	// 바 위젯
	// -- BindWidget: 해당 속성과 같은 이름의 위젯과 바인드
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USBW_Bar", meta = (BindWidget))
	TObjectPtr<UProgressBar> BarWidget;

	// 'BarWidget' 수치 최대값
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USBW_Bar")
	float MaxFigure;
};
