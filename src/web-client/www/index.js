var catIDPrefix = "cat";

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
		
		// Check for global variables
		if(!("configVars" in window))
		{
			alert("Error: 'configVars' could not be found.");
		}
		else if(!("strings" in window))
		{
			alert("Error: 'strings' could not be found.");
		}
		else
		{
			listCategories();
		}
	}
);

function serverError(data)
{
	console.log("Server Error\nError code: '" + data.error_code + "'");
	alert("Server Error\nError code: '" + data.error_code + "'");
}

function clientError(message)
{
	console.log("Error\n" + message);
	alert("Error\n" + message);
}

function resizeContainer()
{
	$("#container").height($(window).height() - 50);
}

function listCategories()
{
	$("#categoryList").html(strings.loadingText);
		
	$.get(configVars.serverAddress, {"action": "list_categories"}, displayCategoryList, "json");
}

function displayCategoryList(data)
{
	if(data.status == "ok")
	{
		var output =
			"<div id='addCategoryButton' class='button'>" + strings.addCategoryButtonText + "</div>" +
			"<ul class='collapsibleList'>";
		
		// Alphabetize the list
		data.cats.sort(sortByObjectName);
		
		for(i = 0; i < data.cats.length; i++)
		{
			output += createRecursiveList(data.cats[i]);
		}
		
		output += "</ul>";
		
		CollapsibleLists.applyTo($("#categoryList").html(output)[0]);
		
		// Bind click event
		$("span.categoryLabel").unbind("click").click(
			function(event)
			{
				var id = $(event.target).parent().attr("id").substring(catIDPrefix.length);
				
				$("#tabContainer").html(displayCategory(getCategoryByID(id, data.cats)));
			}
		);
		
		$("#addCategoryButton").toggle(
			function(event)
			{
				var buttonPosition = $(event.target).offset();
				var buttonHeight = $(event.target).outerHeight();
				
				var windowLeft = buttonPosition.left;
				var windowTop = buttonPosition.top + buttonHeight;
				
				var windowHeight = $("#addCategoryWindow").outerHeight();
				
				if(windowTop + windowHeight > $(window).height())
				{
					windowTop = buttonPosition.top - windowHeight;
				}
				
				$(event.target).html(strings.cancelText);
				
				$("#addCategoryWindow_name").attr("value", "");
				
				$("#addCategoryWindow_parent").html("<option>" + strings.loadingText + "</option>");
				
				$("#addCategoryWindow_submit").html(strings.addCategorySubmitText).addClass("disabled");
				
				$.get(configVars.serverAddress, {"action": "list_categories"}, fillCategoriesDropDown, "json");
				
				$("#addCategoryWindow").css(
					{
						"left": windowLeft + "px",
						"top": windowTop + "px"
					}
				).show();
			},
			
			function(event)
			{
				$(event.target).html(strings.addCategoryButtonText);
				
				$("#addCategoryWindow").hide();
			}
		);
		
		$("ul.collapsibleList").find("li").mouseover(
			function(event)
			{
				$(this).children("span.modifyCategoryButton").show();
				
				return false;
			}
		)
		.mouseout(	
			function(event)
			{
				$(this).children("span.modifyCategoryButton").hide();
			}
		);
		
		$("span.modifyCategoryButton").toggle(
			function(event)
			{
				$(event.target).html(strings.cancelText);
				
				var windowDiv = $(event.target).siblings("div.modifyCategoryWindow").eq(0);
				
				var buttonPosition = $(event.target).offset();
				var buttonHeight = $(event.target).outerHeight();
				
				var windowLeft = buttonPosition.left;
				var windowTop = buttonPosition.top + buttonHeight;
				
				var windowHeight = windowDiv.outerHeight();
				
				// Make sure it's not sticking below the bottom of the viewport
				if(windowTop + windowHeight > $(window).height())
				{
					windowTop = buttonPosition.top - windowHeight;
				}
				
				// Bind button click events
				windowDiv.children("span.renameCategoryButton").unbind("click").click(renameCategoryClickHandler);
				windowDiv.children("span.deleteCategoryButton").unbind("click").click(deleteCategoryClickHandler);
				
				windowDiv.css({"left": windowLeft + "px", "top": windowTop + "px"})
					.show()
					.unbind("click")
					.click(
						function(event)
						{
							return false;
						}
					);
				
				return false;
			},
			
			function(event)
			{
				$(event.target).html(strings.modifyCategoryButtonText);
				
				var windowDiv = $(event.target).siblings("div.modifyCategoryWindow").eq(0);
				
				windowDiv.hide();
				
				windowDiv.children("span.renameCategoryButton").unbind("click");
				windowDiv.children("span.deleteCategoryButton").unbind("click");
				
				return false;
			}
		);
	}
	else
	{
		serverError(data);
	}
}

function createRecursiveList(category)
{
	var output = "<li id='" + catIDPrefix + category.id + "'>";
	
	output +=
		"<span class='categoryLabel'>" + category.name + "</span>" +
		"<span class='modifyCategoryButton button'>" + strings.modifyCategoryButtonText + "</span>" +
		"<div class='modifyCategoryWindow'>" +
			"<span class='renameCategoryButton button' id='renameCategoryButton_" + category.id + "'>" + strings.renameCategoryButtonText + "</span>" +
			"<br />" +
			"<span class='deleteCategoryButton button' id='deleteCategoryButton_" + category.id + "'>" + strings.deleteCategoryButtonText + "</span>" +
		"</div>";
	
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

function renameCategoryClickHandler(event)
{
	
}

function deleteCategoryClickHandler(event)
{
	var catID = $(event.target).attr("id");
	catID = catID.substring(catID.indexOf("_") + 1);
	
	deleteCategory(catID);
}

function fillCategoriesDropDown(data)
{
	if(data.status == "ok")
	{
		var output = "<option value='-1'>None</option>";
		
		data.cats.sort(sortByObjectName);
		
		for(var i = 0; i < data.cats.length; i++)
		{
			output += createRecursiveDropDownOptions(data.cats[i], 0);
		}
		
		$("#addCategoryWindow_parent").html(output);
		
		$("#addCategoryWindow_submit").removeClass("disabled").unbind("click").click(
			function(event)
			{
				var name = $("#addCategoryWindow_name").attr("value");
				var parentID = $("#addCategoryWindow_parent").attr("value");
				
				if(name != undefined && name.replace(/ /g, "") != "")
				{
					addCategory(name, parentID);
					
					$(event.target).unbind("click");
					
					$("#addCategoryButton").click();
				}
				else
				{
					alert("Invalid Name");
				}
			}
		);
	}
	else
	{
		serverError(data);
	}
}

function createRecursiveDropDownOptions(category, level)
{
	var output = "<option value='" + category.id + "'>";
	
	for(var j = 0; j < level; j++)
	{
		output += "&nbsp;&nbsp;&nbsp;&nbsp;";
	}
	
	output += category.name + "</option>";
	
	category.children.sort(sortByObjectName);
	
	if(category.children.length > 0)
	{
		for(var j = 0; j < category.children.length; j++)
		{
			output += createRecursiveDropDownOptions(category.children[j], level + 1);
		}
	}
	
	return output;
}

function addCategory(name, parentID)
{
	// Don't do it if there are invalid values
	if(name != undefined && name.replace(/ /g, "") != "" && parent != undefined)
	{
		console.log("Adding category: {name: \"" + name + "\", parentID: \"" + parentID + "\"}");
		
		$.post(configVars.serverAddress + "?action=add_category", {"catname": name, "parent": parentID}, addCategorySuccess, "json");
	}
	else
	{
		clientError("'name' and/or 'parent' is undefined or invalid");
	}
}

function addCategorySuccess(data)
{
	console.log(data);		// {"status":"ok","catid":57}
	
	if(data.status == "ok")
	{
		
	}
	else
	{
		serverError(data);
	}
	
	listCategories();	
}

function renameCategory(catID, name)
{
	
}

function deleteCategory(catID)
{
	if($.isNumeric(catID))
	{
		console.log("Deleting category: {catid: \"" + catID + "\"}");
		
		$.post(configVars.serverAddress + "?action=delete_category", {"catid": catID}, deleteCategorySuccess, "json");
	}
	else
	{
		clientError("'catID' is not a number: " + catID)
	}
}

function deleteCategorySuccess(data)
{
	console.log(data);		// {"status":"ok"}
	
	if(data.status == "ok")
	{
		
	}
	else
	{
		serverError(data);
	}
	
	listCategories();	
}