
```
HADFile             := RootObjectDef

RootObjectDef       := Keyword_Ui <identifer> '{' PropertyList '}' ';'
                    := ObjectFwdDef RootObjectDef;

ObjectDef           := ObjectFwdDef
                    := RealObjectDef

RealObjectDef       := ClassName <identifier> '{' PropertyList '}' ';'

ObjectFwdDef        := ClassName <identifier> ';'

PropertyList        := PropertyListEntry
                    := PropertyListEntry PropertyList

PropertyListEntry   := PropertyDef
                    := ObjectDef
                    := ContentList
                    := ContentRef

PropertyDef         := <identifier> PropertyValue ';'

PropertyValue       := Keyword_True
                    := Keyword_False
                    := <identifier>
                    := TranslatedString

TranslatedString    := '"' AnyChars '"'

ContentList         := Keyword_Content '[' ContentRefs ']'

ContentRefs         := ContentRef
                    := ContentRefs ContentRef

ContentRef          := ObjectDef
                    := Separator

ClassName           := Keyword_Action
                    := Keyword_Menu
                    := Keyword_MenuBar
                    := Keyword_ToolBar
                    := Keyword_MergePoint
                    := Keyword_Container

Separator           := Keyword_Separator ';'

Keyword_True        := "true"
Keyword_False       := "false"
Keyword_Content     := "Content"
Keyword_Ui          := "Ui"
Keyword_Action      := "Action"
Keyword_Menu        := "Menu"
Keyword_MenuBar     := "MenuBar"
Keyword_ToolBar     := "ToolBar"
Keyword_Separator   := "Sep"
                    := "Separator"
Keyword_MergePlace  := "MergePlace"
Keyword_Container   := "Container"

AnyChars            := >> Every Char other than " <<
```
