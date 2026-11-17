// Ashley


#include "InvestigationBoard/Ashley_InvestigationBoardDefinition.h"
#include "DataAssets/Ashley_PickupDA.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#define LOCTEXT_NAMESPACE "InvestigationBoardDefinition"

TArray<FName> UInvestigationBoardDefinition::GetValidIDs() const
{
	// None first, so the field can be cleared (no item required)
	TArray<FName> IDs = { NAME_None };

	if (const UAshley_PickupDA* LoadedAsset = PickupDataAsset.LoadSynchronous())
	{
		IDs.Append(LoadedAsset->GetIDsValids());
	}

	return IDs;
}

TArray<FName> UInvestigationBoardDefinition::GetPostItIds() const
{
	TArray<FName> IDs;

	for (const FInvestigationPostIt& PostIt : PostIts)
	{
		if (!PostIt.Id.IsNone())
			IDs.AddUnique(PostIt.Id);
	}

	return IDs;
}

#if WITH_EDITOR
EDataValidationResult UInvestigationBoardDefinition::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	const TArray<FName> PostItIds = GetPostItIds();

	auto CheckEnd = [&](FName End, const FText& Where)
	{
		if (!PostItIds.Contains(End))
		{
			Context.AddError(FText::Format(
				LOCTEXT("UnknownPostIt", "{0}: '{1}' is not a post-it of this board"),
				Where, FText::FromName(End)));
			Result = EDataValidationResult::Invalid;
		}
	};

	for (const FInvestigationConnection& Connection : Connections)
	{
		const FText Where = LOCTEXT("ConnectionsWhere", "Connections");
		CheckEnd(Connection.A, Where);
		CheckEnd(Connection.B, Where);
	}

	for (const FInvestigationPostIt& PostIt : PostIts)
	{
		if (!PostIt.bRequireDeduction)
			continue;

		const FText Where = FText::Format(
			LOCTEXT("RequiredWhere", "Post-it '{0}' RequiredConnections"), FText::FromName(PostIt.Id));

		for (const FInvestigationConnection& Required : PostIt.RequiredConnections)
		{
			CheckEnd(Required.A, Where);
			CheckEnd(Required.B, Where);

			// TryConnect only accepts connections listed in Connections, so this one could never be made
			if (!Connections.Contains(Required))
			{
				Context.AddError(FText::Format(
					LOCTEXT("RequiredNotInConnections", "{0}: '{1}' - '{2}' is not in Connections"),
					Where, FText::FromName(Required.A), FText::FromName(Required.B)));
				Result = EDataValidationResult::Invalid;
			}
		}
	}

	if (Result == EDataValidationResult::NotValidated)
		Result = EDataValidationResult::Valid;

	return Result;
}
#endif

#undef LOCTEXT_NAMESPACE
