#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Brawlerf.generated.h"

UCLASS()
class UF2_API ABrawlerf : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	ABrawlerf();
	virtual void SetTarget() override;
	virtual void ResetTarget() override;
	virtual void SetName() override;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BrawlerMesh;

};
