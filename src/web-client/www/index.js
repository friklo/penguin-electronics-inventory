$(document).ready(
	function()
	{
		console.log("Server URL: " + configVars.serverAddress);
		
		resizeContainer();
		
		$(window).resize(resizeContainer);
		
		$("#listCategoriesButton").click(listCategories);
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
	
	console.log(data);
	
	for(i = 0; i < data.cats.length; i++)
	{
		output += createRecursiveList(data.cats[i]);
	}
	
	output += "</ul>";
	
	CollapsibleLists.applyTo($("#categoryList").html(output)[0]);
}

function createRecursiveList(category)
{
	var output = "<li>";
	
	output += category.name;
	
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