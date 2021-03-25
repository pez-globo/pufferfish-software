import { Button, Grid, TableCell, TableRow, Typography } from '@material-ui/core';
import { createStyles, makeStyles, useTheme } from '@material-ui/core/styles';
import React, { useCallback, useEffect } from 'react';
import { shallowEqual, useDispatch, useSelector } from 'react-redux';
import { updateCommittedState } from '../../store/controller/actions';
import { LogEvent, LogEventType, Range } from '../../store/controller/proto/mcu_pb';
import {
  getActiveLogEventIds,
  getAlarmLimitsRequest,
  getNextLogEvents,
} from '../../store/controller/selectors';
import { EXPECTED_LOG_EVENT_ID } from '../../store/controller/types';
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
 * LogsPage
 *
 * TODO: Abstract this into components!
 */

interface Data {
  type: LogEventType;
  alarm: string;
  time: number; // Note: Make this a date object?
  status: number;
  id: number;
  details: string;
  stateKey: string;
  head: string;
  unit: string;
}

//

const headCells: HeadCell[] = [
  { id: 'type', numeric: false, disablePadding: true, label: 'Type', enableSort: false },
  { id: 'alarm', numeric: true, disablePadding: false, label: 'Alarm', enableSort: false },
  { id: 'time', numeric: true, disablePadding: false, label: 'Time/Date', enableSort: true },
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
      boxShadow: 'none !important',
      padding: '0rem 3rem !important',
      border: 'none',
      color: '#fff',
    },
  }),
);

/**
 * LogsPage
 */
export const LogsPage = ({ filter }: { filter?: boolean }): JSX.Element => {
  const classes = useStyles();
  const theme = useTheme();
  const dispatch = useDispatch();

  const getEventTypeLabel = (type: LogEventType): string => {
    switch (type) {
      case LogEventType.patient:
        return 'Patient';
      case LogEventType.control:
        return 'Control';
      case LogEventType.system:
        return 'System';
      default:
        return 'System';
    }
  };

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

  const [rows, setRows] = React.useState<Data[]>([]);
  const [order, setOrder] = React.useState<Order>('desc');
  const [orderBy, setOrderBy] = React.useState<keyof Data>('time');
  const [selected, setSelected] = React.useState<string[]>([]);
  const [page, setPage] = React.useState(0);
  const [rowsPerPage, setRowsPerPage] = React.useState(8);
  const [open, setOpen] = React.useState(false);
  const [alarmOpen, setAlarmOpen] = React.useState(false);
  const [currentRow, setCurrentRow] = React.useState<Data>();
  const emptyRows = rowsPerPage - Math.min(rowsPerPage, rows.length - page * rowsPerPage);
  const loggedEvents = useSelector(getNextLogEvents, shallowEqual);
  const activeLogEventIds = useSelector(getActiveLogEventIds, shallowEqual);
  const alarmLimits: Record<string, Range> = useSelector(
    getAlarmLimitsRequest,
    shallowEqual,
  ) as Record<string, Range>;
  const settingsAllowed = ['hr', 'spo2'];

  const updateLogEvent = useCallback(
    (maxId) => {
      dispatch(updateCommittedState(EXPECTED_LOG_EVENT_ID, { id: maxId + 1 }));
    },
    [dispatch],
  );

  const getDetails = useCallback(getEventDetails, []);

  useEffect(() => {
    const eventIds: number[] = [];
    const data: Data[] = [];
    loggedEvents.sort((a: LogEvent, b: LogEvent) => a.time - b.time);
    loggedEvents.forEach((event: LogEvent) => {
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
              getDetails(event, eventType, alarmLimits),
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
            getDetails(event, eventType, alarmLimits),
            eventType.stateKey || '',
            eventType.head || '',
            eventType.unit || '',
          ),
        );
      }
    });
    setRows(data.length ? data : []);
    // update ExpectedLogEvent
    updateLogEvent(Math.max(...eventIds));
  }, [loggedEvents, activeLogEventIds, updateLogEvent, filter, alarmLimits, getDetails]);

  const handleClose = () => {
    setOpen(false);
  };

  const handleConfirm = () => {
    handleClose();
  };

  const typeColor = (type: LogEventType | undefined) => {
    if (type === LogEventType.control) return { backgroundColor: theme.palette.primary.main };
    if (type === LogEventType.patient) return { backgroundColor: '#92D25B', color: 'black' };
    if (type === LogEventType.system) return { backgroundColor: '#E68619' };
    return { backgroundColor: '#E68619' };
  };

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
                                        ${new Date(row.time * 1000).toLocaleTimeString([], {
                                          hour: '2-digit',
                                          minute: '2-digit',
                                        })}
                                        ${new Date(row.time * 1000).toLocaleDateString([], {
                                          month: '2-digit',
                                          day: '2-digit',
                                          year: 'numeric',
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
                      style={{ padding: '6px 3rem' }}
                    >
                      Settings
                    </Button>
                  )}
                </TableCell>
              </StyledTableRow>
            );
          })}
        {emptyRows > 0 && (
          <TableRow style={{ height: 53 * emptyRows }}>
            <TableCell colSpan={6} />
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
