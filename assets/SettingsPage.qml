import bb.cascades 1.3

Page {
    id: root

    property int updatePeriod: 5
    property alias stationaryDetection: stationaryDetection.checked
    property alias filterData: filterData.checked
    property alias saveOnMediaCard: saveOnMediaCard.checked

    titleBar: TitleBar {
        title: qsTr("Settings")
        visibility: ChromeVisibility.Visible
    }
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        topPadding: ui.du(2.0)
        leftPadding: topPadding
        rightPadding: leftPadding
        bottomPadding: topPadding
        DropDown {
            title: qsTr("Update Interval")
            options: [
                Option {
                    text: "1 " + qsTr("s")
                },
                Option {
                    text: "2 " + qsTr("s")
                },
                Option {
                    text: "5 " + qsTr("s")
                },
                Option {
                    text: "10 " + qsTr("s")
                },
                Option {
                    text: "30 " + qsTr("s")
                },
                Option {
                    text: "1 " + qsTr("min")
                }
            ]
            function setUpdatePeriod() {
                var opts = [1, 2, 5, 10, 30, 60]
                root.updatePeriod = opts[selectedIndex]
            }
            onSelectedIndexChanged: {
                setUpdatePeriod()
                _settings.setValue("updatePeriodIndex", selectedIndex)
            }
            onCreationCompleted: {
                selectedIndex = _settings.value("updatePeriodIndex", 2)
                setUpdatePeriod()
            }
        }
        Divider {
        }
        LabelToggleButton {
            id: stationaryDetection
            text: qsTr("Stationary Detection")
            horizontalAlignment: HorizontalAlignment.Fill
            checked: _settings.value("stationaryDetection", false)
            onCheckedChanged: _settings.setValue("stationaryDetection", checked)
        }
        Divider {
        }
        LabelToggleButton {
            id: filterData
            text: qsTr("Filter Location Data")
            horizontalAlignment: HorizontalAlignment.Fill
            checked: _settings.value("filterData", false)
            onCheckedChanged: _settings.setValue("filterData", checked)
        }
        Divider {
        }
        LabelToggleButton {
            id: saveOnMediaCard
            text: qsTr("Save on Media Card")
            horizontalAlignment: HorizontalAlignment.Fill
            checked: _settings.value("saveOnMediaCard", false)
            onCheckedChanged: _settings.setValue("saveOnMediaCard", checked)
        }
    }
}
