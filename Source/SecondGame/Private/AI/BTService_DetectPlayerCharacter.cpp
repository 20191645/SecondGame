// BTService_DetectPlayerCharacter.cpp

#include "AI/BTService_DetectPlayerCharacter.h"
#include "Controller/SAIController.h"
#include "Character/SNonPlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animation/SAnimInstance.h"

UBTService_DetectPlayerCharacter::UBTService_DetectPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Behavior Tree�� ���� ��� �̸� ����
	NodeName = TEXT("DetectPlayerCharacter");
	// TickNode() �Լ� ȣ�� �ֱ� ����
	Interval = 1.f;
}

void UBTService_DetectPlayerCharacter::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASAIController* AIC = Cast<ASAIController>(OwnerComp.GetAIOwner());
	if (IsValid(AIC) == true)
	{
		ASNonPlayerCharacter* NPC = Cast<ASNonPlayerCharacter>(AIC->GetPawn());
		if (IsValid(NPC) == true)
		{
			UWorld* World = NPC->GetWorld();
			if (IsValid(World) == true)
			{
				FVector CenterPosition = NPC->GetActorLocation();
				// �÷��̾� ĳ���� Ž�� �Ÿ�
				float DetectRadius = 2000.f;

				// �ϳ� �̻��� ������Ʈ�� �����ðű� ������ TArray ���
				TArray<FOverlapResult> OverlapResults;
				FCollisionQueryParams CollisionQueryParams(NAME_None, false, NPC);
				// ���� ������Ʈ�� ���� -- {Overlap}{Multi}{ByChannel}
				bool bResult = World->OverlapMultiByChannel(
					OverlapResults,
					CenterPosition,
					FQuat::Identity,
					ECollisionChannel::ECC_GameTraceChannel3,	// = 'Detect' Trace Channel
					FCollisionShape::MakeSphere(DetectRadius),
					CollisionQueryParams
				);

				// ������Ʈ�� ������ ���, Player Character ��������
				if (bResult == true)
				{
					for (auto const& OverlapResult : OverlapResults)
					{
						ASCharacter* PC = Cast<ASCharacter>(OverlapResult.GetActor());
						if (IsValid(PC) == true) {
							// IsPlayerController(): ������ ĳ���Ͱ� �÷��̾ �����ϰ� �ִ� ������ Ȯ��
							if (PC->GetController()->IsPlayerController() == true)
							{
								// �÷��̾� ĳ���� Ž�� ����

								// �÷��̾� ĳ���Ͱ� �׾����� ��� -- Ž�� ����
								USAnimInstance* SAnimInstance = Cast<USAnimInstance>(PC->GetMesh()->GetAnimInstance());
								if (IsValid(SAnimInstance) == true) {
									if (SAnimInstance->GetbIsDead() != 0) {
										OwnerComp.GetBlackboardComponent()->SetValueAsObject(ASAIController::TargetActorKey, nullptr);
										continue;
									}
								}

								// 'BB_NPC'�� 'TargetActor' Key ���� 'PlayerCharacter'�� ����
								OwnerComp.GetBlackboardComponent()->SetValueAsObject(ASAIController::TargetActorKey, PC);
								break;
							}
							else {
								// ������ ĳ���Ͱ� NPC
								// 'BB_NPC'�� 'TargetActor' Key ���� nullptr�� ����
								OwnerComp.GetBlackboardComponent()->SetValueAsObject(ASAIController::TargetActorKey, nullptr);
							}
						}
					}
				}
				else
				{
					// 'BB_NPC'�� 'TargetActor' Key ���� nullptr�� ����
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(ASAIController::TargetActorKey, nullptr);
				}
			}
		}
	}
}
