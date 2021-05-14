/**
 * @summary A short one-line description for the file
 *
 * @file More detailed description for the file, if necessary;
 * perhaps spanning multiple lines.
 *
 */
import { Button, Grid, Tab, Tabs } from '@material-ui/core/';
import { makeStyles, Theme } from '@material-ui/core/styles';
import React from 'react';
import { useDispatch } from 'react-redux';
import { commitRequest } from '../../store/controller/actions';
import {
  FrontendDisplaySetting,
  SystemSettingRequest,
} from '../../store/controller/proto/frontend_pb';
import { MessageType } from '../../store/controller/types';
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
  const [value, setValue] = React.useState(0);
  const dispatch = useDispatch();
  /**
   * State to manage theme setting
   */
  const [displaySetting, setDisplaySetting] = React.useState<
    FrontendDisplaySetting | Record<string, unknown>
  >();
  /**
   * State to manage System Brightness & time setting
   */
  const [systemSetting, setSystemSetting] = React.useState<
    SystemSettingRequest | Record<string, unknown>
  >();

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
  const onSettingChange = (settings: Record<string, unknown>) => {
    setDisplaySetting(
      FrontendDisplaySetting.fromJSON({
        theme: settings.theme,
        unit: settings.unit,
      }),
    );
    setSystemSetting(
      SystemSettingRequest.fromJSON({
        brightness: settings.brightness,
        date: settings.date,
      }),
    );
  };

  /**
   * Function for update System & Display settings to redux store
   */
  const handleSubmit = () => {
    dispatch(
      commitRequest<FrontendDisplaySetting>(
        MessageType.FrontendDisplaySetting,
        displaySetting as Record<string, unknown>,
      ),
    );
    dispatch(
      commitRequest<SystemSettingRequest>(
        MessageType.SystemSettingRequest,
        systemSetting as Record<string, unknown>,
      ),
    );
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
