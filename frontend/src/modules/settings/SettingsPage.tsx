/**
 * @summary Component to manage System Info, Settings and Test & Calibaratin
 *
 * @file It is displayed it different tabs for each
 *
 */
import { Button, Grid, Tab, Tabs } from '@material-ui/core/';
import { makeStyles, Theme } from '@material-ui/core/styles';
import React, { useCallback } from 'react';
import { useDispatch, useSelector } from 'react-redux';
import { commitRequest } from '../../store/controller/actions';
import { getSystemSettingsCurrentSeqNum } from '../../store/controller/selectors/frontend_pb';
import { FrontendDisplaySetting, SystemSettingsRequest } from '../../store/proto/frontend_pb';
import { MessageType } from '../../store/proto/types';
import { a11yProps, TabPanel } from '../controllers/TabPanel';
import { DisplayTab, InfoTab, TestCalibrationTab } from './containers';

const useStyles = makeStyles((theme: Theme) => ({
  tabPanelContainer: {
    flexGrow: 1,
    justifyContent: 'space-between',
    alignItems: 'stretch',
    height: '100',
    // border: '1px solid green',
  },
  tabs: {
    // paddingTop: theme.spacing(1)
    // border: '1px solid red',
    width: '100%',
  },
  tab: {
    borderRadius: 8,
    border: `2px solid ${theme.palette.primary.main}`,
    margin: '0px 8px',
    zIndex: 1,
    minHeight: 40,
    minWidth: 220,
  },
  selectedTab: { color: theme.palette.primary.contrastText },
  tabIndicator: {
    borderRadius: 8,
    border: `2px solid ${theme.palette.primary.main}`,
    zIndex: 0,
    marginBottom: theme.spacing(1),
    minHeight: 40,
    background: theme.palette.primary.main,
  },
}));

enum TabType {
  INFO_TAB,
  TEST_CALIBRATION_TAB,
  DISPLAY_TAB,
}

/**
 * SettingsPage
 *
 * @component A container storing various setting panels.
 *
 * @returns {JSX.Element}
 *
 */
export const SettingsPage = (): JSX.Element => {
  const classes = useStyles();
  /**
   * State to manage active Tab
   */
  const [value, setValue] = React.useState(0);
  const dispatch = useDispatch();
  /**
   * State to manage theme setting
   */
  const [displaySetting, setDisplaySetting] = React.useState<FrontendDisplaySetting>();
  /**
   * State to manage System Brightness & time setting
   */
  const [systemSettingsRequest, setSystemSettingsRequest] = React.useState<SystemSettingsRequest>();

  const systemSettingsSeqNum = useSelector(getSystemSettingsCurrentSeqNum);

  /**
   * function for handling tab change.
   *
   * @param {React.ChangeEvent<Record<string, unknown>>} event DOM Change Event
   * @param {number} newValue New tab index value
   *
   */
  const handleChange = (event: React.ChangeEvent<Record<string, unknown>>, newValue: number) => {
    setValue(newValue);
  };

  /**
   * Function for updating System & Display Settings
   *
   * @param {Record<string, unknown>} settings System & Display Settings object
   *
   */
  const onSettingChange = useCallback(
    (settings: Record<string, unknown>) => {
      setDisplaySetting(
        FrontendDisplaySetting.fromJSON({
          theme: settings.theme,
          unit: settings.unit,
        }),
      );
      if (systemSettingsSeqNum === null) {
        console.error('SystemSettings was not initialized!');
        return;
      }
      setSystemSettingsRequest(
        SystemSettingsRequest.fromJSON({
          displayBrightness: settings.displayBrightness,
          date: settings.date,
          seqNum: (systemSettingsSeqNum + 1) % 2 ** 32,
        }),
      );
    },
    [setDisplaySetting, setSystemSettingsRequest, systemSettingsSeqNum],
  );

  /**
   * Function for update System & Display settings to redux store
   */
  const handleSubmit = () => {
    if (displaySetting !== undefined) {
      dispatch(
        commitRequest<FrontendDisplaySetting>(MessageType.FrontendDisplaySetting, displaySetting),
      );
    }
    if (systemSettingsRequest !== undefined) {
      dispatch(
        commitRequest<SystemSettingsRequest>(
          MessageType.SystemSettingsRequest,
          systemSettingsRequest,
        ),
      );
    }
  };

  return (
    <Grid container direction="column">
      <Grid container className={classes.tabPanelContainer}>
        <TabPanel value={value} index={TabType.INFO_TAB}>
          <InfoTab />
        </TabPanel>
        <TabPanel value={value} index={TabType.TEST_CALIBRATION_TAB}>
          <TestCalibrationTab />
        </TabPanel>
        <TabPanel value={value} index={TabType.DISPLAY_TAB}>
          <DisplayTab onSettingChange={onSettingChange} />
        </TabPanel>
      </Grid>
      <Grid container item justify="center" alignItems="center">
        <Tabs
          value={value}
          onChange={handleChange}
          className={classes.tabs}
          classes={{ indicator: classes.tabIndicator }}
        >
          <Tab
            label="Info"
            {...a11yProps(0)}
            className={classes.tab}
            classes={{ selected: classes.selectedTab }}
          />
          <Tab
            label="Test & Calibration"
            {...a11yProps(1)}
            className={classes.tab}
            classes={{ selected: classes.selectedTab }}
          />
          <Tab
            label="Display"
            {...a11yProps(2)}
            className={classes.tab}
            classes={{ selected: classes.selectedTab }}
          />
          <Grid container item justify="flex-end" alignItems="center">
            {value === TabType.DISPLAY_TAB && (
              <Button onClick={() => handleSubmit()} variant="contained" color="secondary">
                Apply Changes
              </Button>
            )}
          </Grid>
        </Tabs>
      </Grid>
    </Grid>
  );
};

export default SettingsPage;
