// SBW_Bar.cpp

#include "UI/SBW_Bar.h"
#include "Components/ProgressBar.h"

USBW_Bar::USBW_Bar(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void USBW_Bar::SetMaxFigure(float InMaxFigure)
{
	if (InMaxFigure < KINDA_SMALL_NUMBER)
	{
		MaxFigure = 0.f;
		return;
	}

	MaxFigure = InMaxFigure;
}