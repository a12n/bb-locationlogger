import bb.cascades 1.3

NavigationPane {
    id: navPane

    attachedObjects: [
        SettingsPage {
            id: settingsPage
            onPeriodChanged: _geoLocation.setPeriod(period)
            onFilterDataChanged: _geoLocation.setFilterData(filterData)
            onStationaryDetectionChanged: _geoLocation.setStationaryDetection(stationaryDetection)
            onSaveOnMediaCardChanged: _gpxFile.setSaveOnMediaCard(saveOnMediaCard)
        }
    ]

    Menu.definition: MenuDefinition {
        helpAction: HelpActionItem {
        }
        settingsAction: SettingsActionItem {
            onTriggered: {
                navPane.push(settingsPage)
            }
        }
    }

    MainPage {
    }
}
