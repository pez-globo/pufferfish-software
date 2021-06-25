/**
 * @summary Events logs listing page popup
 *
 *
 */
import { Button, Grid, TableCell, TableRow, Typography } from '@material-ui/core';
import { createStyles, makeStyles, useTheme } from '@material-ui/core/styles';
import React, { useCallback, useEffect } from 'react';
import { shallowEqual, useSelector } from 'react-redux';
import { LogEvent, LogEventType } from '../../store/controller/proto/mcu_pb';
import { getActiveLogEventIds, getNextLogEvents } from '../../store/controller/selectors';
import { setMultiPopupOpen } from '../app/Service';
import { AlarmModal } from '../controllers';
import ModalPopup from '../controllers/ModalPopup';

import SimpleTable, {
  getComparator,
  HeadCell,
  Order,
  stableSort,
  StyledTableRow,
} from '../controllers/SimpleTable';
import EventlogDetails from './container/EventlogDetails';
import { getEventDetails, getEventType } from './EventType';

/**
 * @typedef Data
 *
 * Interface to create log data.
 *
 * @prop {LogEventType} type  Event Alarm type
 * @prop {string} alarm Event Alarm Label
 * @prop {number} time Event Alarm timestamp
 * @prop {number} status Active status of the alarm
 * @prop {number} id unique identifier of row
 * @prop {number} details Event Alarm details
 * @prop {string} stateKey Event log parameter measurements identifier
 * @prop {string} head Label passed to `AlarmModal` Label prop
 * @prop {string} unit Unit measurement of `stateKey`
 *
 */

interface Data {
  type: LogEventType;
  alarm: string;
  time: number; // Note: Make this a date object? (yes could be Date object, its in timestamp right now)
  status: number;
  id: number;
  details: string;
  stateKey: string;
  head: string;
  unit: string;
}

// Table Header Configuration

const headCells: HeadCell[] = [
  { id: 'type', numeric: false, disablePadding: true, label: 'Type', enableSort: false },
  { id: 'alarm', numeric: true, disablePadding: false, label: 'Event', enableSort: false },
  { id: 'time', numeric: true, disablePadding: false, label: 'Time/Date', enableSort: false },
  { id: 'details', numeric: false, disablePadding: false, label: 'Details', enableSort: false },
  { id: 'settings', numeric: true, disablePadding: false, label: 'Settings', enableSort: false },
];

const useStyles = makeStyles(() =>
  createStyles({
    root: {
      width: '100%',
      // border: '1px solid yellow',
    },
    typeWrapper: {
      width: '50%',
      textAlign: 'center',
      borderRadius: 3,
      padding: 4,
    },
    typeWrapper2: {
      fontSize: 16,
      textAlign: 'center',
      borderRadius: 3,
      padding: 4,
    },
    alertSound: {
      backgroundColor: '#ff3b30 !important',
      boxShadow: 'none !important',
      padding: '0rem 0rem !important',
      maxWidth: 40,
      color: '#fff',
    },
    eventType: {
      width: '10rem',
      boxShadow: 'none !important',
      padding: '2px 2rem !important',
      border: 'none',
      color: '#fff',
    },
  }),
);

/**
 * LogsPage
 *
 * @component LogsPage
 *
 * TODO: Abstract this into components!
 *
 * @param {boolean} filter Filters Active event alarms
 *
 * @returns {JSX.Element}
 *
 */
export const LogsPage = ({ filter }: { filter?: boolean }): JSX.Element => {
  const classes = useStyles();
  const theme = useTheme();

  /**
   * function to get event type label
   *
   * @param {LogEventType} type Event Log type
   *
   * @returns {string}
   *
   */
  const getEventTypeLabel = (type: LogEventType): string => {
    switch (type) {
      case LogEventType.patient:
        return 'Patient';
      case LogEventType.system:
        return 'System';
      case LogEventType.control:
        return 'Control';
      case LogEventType.alarm_limits:
        return 'Alarm Limits';
      default:
        return 'System';
    }
  };

  /**
   *  Formulates Row data format
   *
   * @param {LogEventType} type  Event Alarm type
   * @param {string} alarm Event Alarm Label
   * @param {number} time Event Alarm timestamp
   * @param {number} status Active status of the alarm
   * @param {number} id unique identifier of row
   * @param {number} details Event Alarm details
   * @param {string} stateKey Event log parameter measurements identifier
   * @param {string} head Label passed to `AlarmModal` Label prop
   * @param {string} unit Unit of `stateKey`
   *
   * @returns {Data} Row Data
   *
   */

  const createData = (
    type: LogEventType,
    alarm: string,
    time: number,
    status: number,
    id: number,
    details: string,
    stateKey: string,
    head: string,
    unit: string,
  ): Data => {
    return { type, alarm, time, status, id, details, stateKey, head, unit };
  };

  /**
   * State to manage row data(array of all rows) in table
   */
  const [rows, setRows] = React.useState<Data[]>([]);
  /**
   * State to manage order type
   */
  const [order, setOrder] = React.useState<Order>('desc');
  /**
   * State to manage order by column
   */
  const [orderBy, setOrderBy] = React.useState<keyof Data>('id');
  /**
   * State to manage row selection
   * TODO: decide whether to keep or remove
   */
  const [selected, setSelected] = React.useState<string[]>([]);
  /**
   * State to manage pagination
   */
  const [page, setPage] = React.useState(0);
  /**
   * State to manage rows per page
   */
  const [rowsPerPage, setRowsPerPage] = React.useState(9);
  /**
   * State to manage opening of LogsPage modal
   */
  const [open, setOpen] = React.useState(false);
  /**
   * State to manage Alarm Modal open status
   */
  const [alarmOpen, setAlarmOpen] = React.useState(false);
  /**
   * State to manage current selected row
   * To open "Settings"/Alarm Modal for that selection
   */
  const [currentRow, setCurrentRow] = React.useState<Data>();
  const emptyRows = rowsPerPage - Math.min(rowsPerPage, rows.length - page * rowsPerPage);
  const loggedEvents = useSelector(getNextLogEvents, shallowEqual);
  const activeLogEventIds = useSelector(getActiveLogEventIds, shallowEqual);
  /**
   * List to determine which paramter is allowed to show Settings button
   */
  const settingsAllowed = ['fio2', 'flow', 'hr', 'spo2'];

  const getDetails = useCallback(getEventDetails, []);
  /**
   * Initialized & Updates Row data
   */
  useEffect(() => {
    const eventIds: number[] = [];
    const data: Data[] = [];
    const loggedEventsCopy = [...loggedEvents];
    loggedEventsCopy.reverse();
    loggedEventsCopy.forEach((event: LogEvent) => {
      const eventType = getEventType(event.code);
      eventIds.push(event.id);
      if (filter) {
        setPage(0);
        if (activeLogEventIds.indexOf(event.id) > -1) {
          data.push(
            createData(
              eventType.type,
              eventType.label,
              event.time,
              activeLogEventIds.indexOf(event.id) > -1 ? 1 : 0,
              event.id,
              getDetails(event, eventType),
              eventType.stateKey || '',
              eventType.head || '',
              eventType.unit || '',
            ),
          );
        }
      } else {
        data.push(
          createData(
            eventType.type,
            eventType.label,
            event.time,
            activeLogEventIds.indexOf(event.id) > -1 ? 1 : 0,
            event.id,
            getDetails(event, eventType),
            eventType.stateKey || '',
            eventType.head || '',
            eventType.unit || '',
          ),
        );
      }
    });
    setRows(data.length ? data : []);
  }, [loggedEvents, activeLogEventIds, filter, getDetails]);

  /**
   * Function for handling the close popup.
   */
  const handleClose = () => {
    setOpen(false);
  };

  /**
   * Function for handling  confirmation popup.
   */
  const handleConfirm = () => {
    handleClose();
  };

  /**
   * Function returning background color based on Alarm log type.
   *
   * @param {LogEventType | undefined} type desc for type
   *
   * @returns {object} some description
   *
   */
  const typeColor = (type: LogEventType | undefined) => {
    switch (type) {
      case LogEventType.patient:
        return { backgroundColor: '#FF3B30' };
      case LogEventType.system:
        return { backgroundColor: '#E68619' };
      case LogEventType.control:
      case LogEventType.alarm_limits:
        return { backgroundColor: theme.palette.primary.main };
      default:
        return { backgroundColor: '#E68619' };
    }
  };

  /**
   * Trigger row slection on click at row
   *
   * @param {React.MouseEvent<unknown>} event Mouse Event
   * @param {string} name Column name
   *
   */
  const handleClick = (event: React.MouseEvent<unknown>, name: string) => {
    const selectedIndex = selected.indexOf(name);
    let newSelected: string[] = [];

    if (selectedIndex === -1) {
      newSelected = newSelected.concat(selected, name);
    } else if (selectedIndex === 0) {
      newSelected = newSelected.concat(selected.slice(1));
    } else if (selectedIndex === selected.length - 1) {
      newSelected = newSelected.concat(selected.slice(0, -1));
    } else if (selectedIndex > 0) {
      newSelected = newSelected.concat(
        selected.slice(0, selectedIndex),
        selected.slice(selectedIndex + 1),
      );
    }
    setSelected(newSelected);
  };

  /**
   * Function for opening multistep popup with current row `stateKey`
   *
   * @param {Data} row Row data
   *
   */
  const onSettings = (row: Data) => {
    if (row.stateKey) {
      setMultiPopupOpen(true, row.stateKey);
    }
    setCurrentRow(row);
  };

  return (
    <Grid
      container
      direction="column"
      justify="space-between"
      alignItems="stretch"
      className={classes.root}
    >
      <SimpleTable
        order={order}
        setOrder={setOrder}
        orderBy={orderBy}
        setOrderBy={setOrderBy}
        selected={selected}
        setSelected={setSelected}
        page={page}
        setPage={setPage}
        rowsPerPage={rowsPerPage}
        setRowsPerPage={setRowsPerPage}
        headCells={headCells}
        rows={rows}
      >
        {stableSort(rows, getComparator(order, orderBy))
          .slice(page * rowsPerPage, page * rowsPerPage + rowsPerPage)
          .map((row, index) => {
            const labelId = `enhanced-table-checkbox-${index}`;

            return (
              <StyledTableRow
                hover
                onClick={(event: React.MouseEvent<unknown>) =>
                  handleClick(event, getEventTypeLabel(row.type))
                }
                tabIndex={-1}
                key={row.id}
              >
                <TableCell align="left" component="th" id={labelId} scope="row">
                  <Button
                    variant="contained"
                    className={classes.eventType}
                    style={typeColor(row.type)}
                  >
                    {getEventTypeLabel(row.type)}
                  </Button>
                  {/* {row.status ? (
                    <Button
                      style={{ marginLeft: 12, padding: '.2rem 0rem !important', minWidth: 30 }}
                      variant="contained"
                      className={classes.alertSound}
                    >
                      <VolumeUpIcon />
                    </Button>
                  ) : (
                    ''
                  )} */}
                </TableCell>
                <TableCell align="left" component="th" id={labelId} scope="row">
                  {row.alarm}
                </TableCell>
                <TableCell align="left">
                  {`
                                        ${new Date(row.time).toLocaleDateString([], {
                                          month: '2-digit',
                                          day: '2-digit',
                                          year: 'numeric',
                                        })}
                                        ${new Date(row.time).toLocaleTimeString([], {
                                          hour: '2-digit',
                                          minute: '2-digit',
                                        })}
                                    `}
                </TableCell>
                <TableCell align="left" component="th" id={labelId} scope="row">
                  {row.details}
                </TableCell>
                <TableCell component="td">
                  {row.type === LogEventType.patient && settingsAllowed.indexOf(row.stateKey) > -1 && (
                    <Button
                      variant="contained"
                      color="primary"
                      onClick={() => onSettings(row)}
                      style={{ padding: '2px 2rem' }}
                    >
                      Settings
                    </Button>
                  )}
                </TableCell>
              </StyledTableRow>
            );
          })}
        {emptyRows > 0 && (
          <TableRow>
            <TableCell colSpan={6} style={{ padding: 0 }} />
          </TableRow>
        )}
      </SimpleTable>
      <ModalPopup
        showCloseIcon={true}
        label={
          <Grid container direction="row" justify="space-around" alignItems="center">
            <Grid xs={2} className={classes.typeWrapper2} style={typeColor(currentRow?.type)}>
              {getEventTypeLabel(currentRow?.type as LogEventType)}
            </Grid>
            <Grid xs={9}>
              <Typography variant="h4">{currentRow?.alarm}</Typography>
            </Grid>
          </Grid>
        }
        open={open}
        onClose={handleClose}
        onConfirm={handleConfirm}
      >
        <Grid item>
          <EventlogDetails />
        </Grid>
      </ModalPopup>
      {currentRow && currentRow.stateKey && (
        <AlarmModal
          onModalClose={() => setAlarmOpen(false)}
          openModal={alarmOpen}
          disableAlarmButton={true}
          label={currentRow.head}
          units={currentRow.unit}
          stateKey={currentRow.stateKey}
          requestCommitRange={() => null}
        />
      )}
    </Grid>
  );
};

export default LogsPage;
