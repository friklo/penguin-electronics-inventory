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
				
				$("#tabContainer").html(categoryToHTML(getCategoryByID(id, data.cats)));
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
				
				$("#addCategoryWindow_name").unbind("keydown");
				
				$.get(configVars.serverAddress, {"action": "list_categories"}, fillCategoriesDropDown, "json");
				
				$("#addCategoryWindow").css(
					{
						"left": windowLeft + "px",
						"top": windowTop + "px"
					}
				).show();
				
				$("#addCategoryWindow_name").focus();
				
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
				if(!$(this).children("span.modifyCategoryButton").hasClass("persistentLabel"))
				{
					$(this).children("span.modifyCategoryButton").hide();
				}
			}
		);
		
		$("span.modifyCategoryButton").toggle(
			function(event)
			{
				$(event.target).html(strings.cancelText).addClass("persistentLabel");
				
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
				windowDiv.children("span.renameCategoryButton").toggle(
					function(event)
					{
						var catID = $(event.target).attr("id");
						catID = catID.substring(catID.lastIndexOf("_") + 1);
						
						$(event.target).hide();
						$(event.target).siblings("span.deleteCategoryButton").hide();
						$(event.target).siblings("br").hide();
						$(event.target).siblings("div.renameCategoryWindow").show();
						
						$(event.target).siblings("div.renameCategoryWindow").find("input").focus();
						
						$("#renameCategoryWindow_submit_" + catID).unbind("click").click(renameCategoryClickHandler);
						
						$(event.target).siblings("div.renameCategoryWindow").unbind("keydown").keydown(
							function(event)
							{
								if(event.which == 13)	// Enter key
								{
									$("#renameCategoryWindow_submit_" + catID).click();
								}
							}
						);
					},
					
					function(event)
					{
						$(event.target).siblings("div.renameCategoryWindow").hide();
						$(event.target).siblings("br").show();
						$(event.target).siblings("span.deleteCategoryButton").show();
						$(event.target).show();
						
						$(event.target).siblings("div.renameCategoryWindow").unbind("keydown");
					}
				);
				
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
					
					// Todo: For some reason normal native focusing upon clicking the text input
					// does not occur. This is a manual workaround
					windowDiv.find("input").unbind("click").click(
						function(event)
						{
							$(event.target).focus();
						}
					);
				
				return false;
			},
			
			function(event)
			{
				$(event.target).html(strings.modifyCategoryButtonText).removeClass("persistentLabel");
				
				var windowDiv = $(event.target).siblings("div.modifyCategoryWindow").eq(0);
				
				if(windowDiv.children("span.renameCategoryButton").is(":hidden"))
				{
					windowDiv.children("span.renameCategoryButton").click();
				}
				
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
		"<span class='modifyCategoryButton button'>" +
			strings.modifyCategoryButtonText +
		"</span>" +
		"<div class='modifyCategoryWindow'>" +
			"<span class='renameCategoryButton button' id='renameCategoryButton_" + category.id + "'>" +
				strings.renameCategoryButtonText +
			"</span>" +
			"<div class='renameCategoryWindow'>" +
				"<span class='label'>New Name:</span>" +
				"<input type='text' id='renameCategory_name_" + category.id + "' />" +
				"<br />" +
				"<span id='renameCategoryWindow_submit_" + category.id + "' class='button'>" +
					strings.renameCategorySubmitText +
				"</span>" +
			"</div>" +
			"<br />" +
			"<span class='deleteCategoryButton button' id='deleteCategoryButton_" +
				category.id + "'>" + strings.deleteCategoryButtonText +
			"</span>" +
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

function categoryToHTML(category)
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
	var catID = $(event.target).attr("id");
	catID = catID.substring(catID.lastIndexOf("_") + 1);
	
	var name = $("#renameCategory_name_" + catID).attr("value");
	
	if(name.replace(/ /g, "") != "")
	{	
		renameCategory(catID, name);
	}
	else
	{
		alert("Invalid Name");
	}
}

function deleteCategoryClickHandler(event)
{
	var catID = $(event.target).attr("id");
	catID = catID.substring(catID.lastIndexOf("_") + 1);
	
	if(confirm("Are you sure you want to delete this category?"))
	{
		deleteCategory(catID);
	}
	else
	{
		// Don't delete it
	}
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
		
		$("#addCategoryWindow_name").unbind("keydown").keydown(
			function(event)
			{
				if(event.which == 13)		// Enter key
				{
					$("#addCategoryWindow_submit").click();
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
	if(name != undefined && name.replace(/ /g, "") != "" && $.isNumeric(parentID))
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
	if($.isNumeric(catID) && name != undefined && name.replace(/ /g, "") != "")
	{
		console.log("Adding category: {catID: \"" + catID + "\", name: \"" + name + "\"}");
		
		$.post(configVars.serverAddress + "?action=rename_category", {"catid": catID, "name": name}, renameCategorySuccess, "json");
	}
	else
	{
		clientError("'catID' and/or 'name' is undefined or invalid.\ncatID: '" + catID + "', name: '" + name + "'");
	}
}

function renameCategorySuccess(data)
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

function deleteCategory(catID)
{
	if($.isNumeric(catID))
	{
		console.log("Deleting category: {catid: \"" + catID + "\"}");
		
		$.post(configVars.serverAddress + "?action=delete_category", {"catid": catID}, deleteCategorySuccess, "json");
	}
	else
	{
		clientError("'catID' is not a number: '" + catID + "'")
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