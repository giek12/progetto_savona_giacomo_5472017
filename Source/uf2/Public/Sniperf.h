#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Sniperf.generated.h"

UCLASS()
class UF2_API ASniperf : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	ASniperf();
	virtual void SetTarget() override;
	virtual void ResetTarget() override;
	virtual void SetName() override;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SniperMesh;

};
