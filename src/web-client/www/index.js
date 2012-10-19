var idPrefix = "cat";

function sortByObjectName(a, b)
{
	var nameA = a.name;
	var nameB = b.name;
	
	if(nameA < nameB)
	{
		return -1;
	}
	else if(nameB < nameA)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

$(document).ready(
	function()
	{
		console.log("Server URL: " + configVars.serverAddress);
		
		resizeContainer();
		
		$(window).resize(resizeContainer);
		
		listCategories();
	}
);

function resizeContainer()
{
	$("#container").height($(window).height() - 50);
}

function listCategories()
{
	if(configVars)
	{
		$("#categoryList").html("Loading…");
		
		$.get(configVars.serverAddress, {"action": "list_categories"}, displayCategories, "json");
	}
}

function displayCategories(data)
{
	var output = "<ul class='collapsibleList'>";
	
	// Alphabetize the list
	data.cats.sort(sortByObjectName);
	
	console.log(data.cats);
	
	for(i = 0; i < data.cats.length; i++)
	{
		output += createRecursiveList(data.cats[i]);
	}
	
	output += "</ul>";
	
	CollapsibleLists.applyTo($("#categoryList").html(output)[0]);
	
	$("span.categoryLabel").unbind("click").click(
		function(event)
		{
			var id = $(event.target).parent().attr("id").substring(idPrefix.length);
			
			$("#tabContainer").html(displayCategory(getCategoryByID(id, data.cats)));
		}
	);
}

function createRecursiveList(category)
{
	var output = "<li id='" + idPrefix + category.id + "'>";
	
	output += "<span class='categoryLabel'>" + category.name + "</span>";
	
	category.children.sort(sortByObjectName);
	
	if(category.children.length > 0)
	{
		output += "<ul>";
		
		for(var j = 0; j < category.children.length; j++)
		{
			output += createRecursiveList(category.children[j]);
		}
		
		output += "</ul>";
	}
	
	return output;
}

function getCategoryByID(id, categories)
{
	for(var h = 0; h < categories.length; h++)
	{
		if(categories[h].id == id)
		{
			return categories[h];
		}
		else if(categories[h].children.length > 0)
		{
			var category = getCategoryByID(id, categories[h].children);
			
			// If it's not null
			if(category)
			{
				return category;
			}
		}
	}
	
	return null;
}

function displayCategory(category)
{
	console.log(category);
	
	if(category)
	{
	
		return "<span>You selected '" + category.name + "'</span>";
	}
	else
	{
		return "Error in 'dislayCategory', null category";
	}
}