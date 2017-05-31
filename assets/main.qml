import bb.cascades 1.4
import bb.system 1.0

NavigationPane {
    id: navPane

    attachedObjects: [
        SettingsPage {
            id: settingsPage
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

    Page {
        id: mainPage

        onCreationCompleted: {
            _geoLocation.dataChanged.connect(onDataChanged)
            _geoLocation.setPeriod(5)
            _geoLocation.startUpdates()
        }

        function onDataChanged() {
            latLabel.text = _geoLocation.latitude + "°"
            lonLabel.text = _geoLocation.longitude + "°"
            altLabel.text = _geoLocation.altitude + " m"
            numSatLabel.text = _geoLocation.numSatellitesUsed + "/" + _geoLocation.numSatellitesTotal
        }

        function dateTimeDifference(beginTs, endTs) {
            var diff = endTs.valueOf() - beginTs.valueOf()
            var ans = {}
            ans.days = ~~(diff / 86400000)
            diff %= 86400000
            ans.hours = ~~(diff / 3600000)
            diff %= 3600000
            ans.minutes = ~~(diff / 60000)
            diff %= 60000
            ans.seconds = ~~(diff / 1000)
            diff %= 1000
            ans.milliseconds = diff
            return ans
        }

        function durationString(beginTs, endTs) {
            var d = dateTimeDifference(beginTs, endTs)
            var ans = ""
            if (d.days > 0) {
                ans += d.days + "d"
            }
            if (d.hours > 0) {
                ans += d.hours + "h"
            }
            if (d.minutes > 0) {
                ans += d.minutes + "min"
            }
            ans += d.seconds + "s"
            return ans
        }

        attachedObjects: [
            SystemToast {
                id: saveToast
            }
        ]

        actions: [
            ActionItem {
                id: startAction
                title: qsTr("Record")
                ActionBar.placement: ActionBarPlacement.Signature
                enabled: true
                function generateFileName(startTime) {
                    return Qt.formatDateTime(startTime, "yyyyMMdd_HHmmss") + ".gpx"
                }
                onTriggered: {
                    saveToast.body = generateFileName(new Date)
                    _gpxFile.open(saveToast.body)
                    startAction.enabled = false
                    stopAction.enabled = true
                }
                imageSource: "asset:///images/ic_location.png"
            },
            ActionItem {
                id: stopAction
                title: qsTr("Stop")
                ActionBar.placement: ActionBarPlacement.OnBar
                enabled: false
                onTriggered: {
                    _gpxFile.close()
                    saveToast.show()
                    startAction.enabled = true
                    stopAction.enabled = false
                }
                imageSource: "asset:///images/ic_stop.png"
            }
        ]

        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            topPadding: ui.du(2.0)
            leftPadding: topPadding
            rightPadding: leftPadding
            bottomPadding: topPadding
            Label {
                id: latLabel
                text: "N/A°"
                textStyle.fontSize: FontSize.XXLarge
                horizontalAlignment: HorizontalAlignment.Fill
                textStyle.textAlign: TextAlign.Center
                textStyle.fontFamily: "Monospace"
            }
            Label {
                id: lonLabel
                text: "N/A°"
                textStyle.fontSize: FontSize.XXLarge
                horizontalAlignment: HorizontalAlignment.Fill
                textStyle.textAlign: TextAlign.Center
                textStyle.fontFamily: "Monospace"
            }
            Label {
                id: altLabel
                text: "N/A m"
                textStyle.fontSize: FontSize.Large
                textStyle.textAlign: TextAlign.Center
                horizontalAlignment: HorizontalAlignment.Fill
                textStyle.fontFamily: "Monospace"
            }
            Label {
                id: numSatLabel
                text: "0/0"
                textStyle.fontSize: FontSize.Large
                textStyle.textAlign: TextAlign.Center
                horizontalAlignment: HorizontalAlignment.Fill
                textStyle.fontFamily: "Monospace"
            }
            Divider {
                horizontalAlignment: HorizontalAlignment.Fill
            }
            Label {
                // TODO: update
                id: trackTimeLabel
                text: "00:00:00"
                textStyle.fontSize: FontSize.XXLarge
                horizontalAlignment: HorizontalAlignment.Fill
                textStyle.textAlign: TextAlign.Center
            }
        }
    }
}
