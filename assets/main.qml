import bb.cascades 1.3

NavigationPane {
    id: navPane

    attachedObjects: [
        SettingsPage {
            id: settingsPage
            onUpdatePeriodChanged: _geoLocation.setPeriod(updatePeriod)
            onFilterDataChanged: _geoLocation.setFilterData(filterData)
            onStationaryDetectionChanged: _geoLocation.setStationaryDetection(stationaryDetection)
            onSaveOnMediaCardChanged: _gpxFile.setSaveOnMediaCard(saveOnMediaCard)
        }
    ]

    Menu.definition: MenuDefinition {
        settingsAction: SettingsActionItem {
            onTriggered: {
                navPane.push(settingsPage)
            }
        }
    }

    MainPage {
    }
}
