/**
 * @deprecated
 * @summary Component to display Event log details
 *
 *
 */
import { Grid, TableCell, TableRow } from '@material-ui/core';
import { useTheme } from '@material-ui/core/styles';
import React from 'react';
import SimpleTable, {
  getComparator,
  HeadCell,
  Order,
  stableSort,
  StyledTableRow,
} from '../../shared/SimpleTable';

/**
 * @typedef Data
 *
 * Interface for event log details.
 *
 * @prop {string} value Display Value of Event log
 * @prop {string} measurement Unit measurement of event
 * @prop {number} id Event log Id
 *
 */
interface Data {
  value: string;
  measurement: string;
  id: number;
}

/**
 * Function to create row data to display in table
 *
 * @prop {string} value Display Value of Event log
 * @prop {string} measurement Unit measurement of event
 * @prop {number} id Event log Id
 *
 * @returns {Data}
 *
 */
function createData(value: string, measurement: string, id: number): Data {
  return { value, measurement, id };
}

const rows = [
  createData('Respiratory Rate', '18 b/min', 1),
  createData('Respiratory Rate', '18 b/min', 2),
  createData('Respiratory Rate', '18 b/min', 3),
  createData('Respiratory Rate', '18 b/min', 4),
  createData('Respiratory Rate', '18 b/min', 5),
  createData('Respiratory Rate', '18 b/min', 6),
  createData('Respiratory Rate', '18 b/min', 7),
  createData('Respiratory Rate', '18 b/min', 8),
  createData('Respiratory Rate', '18 b/min', 9),
  createData('Respiratory Rate', '18 b/min', 10),
  createData('Respiratory Rate', '18 b/min', 11),
  createData('Respiratory Rate', '18 b/min', 12),
  createData('Respiratory Rate', '18 b/min', 13),
  createData('Respiratory Rate', '18 b/min', 14),
  createData('Respiratory Rate', '18 b/min', 15),
];

const headCells: HeadCell[] = [
  { id: 'value', numeric: false, disablePadding: true, label: 'Value', enableSort: false },
  {
    id: 'measurement',
    numeric: false,
    disablePadding: false,
    label: 'Measurement',
    enableSort: false,
  },
];

/**
 * EventlogDetails
 *
 * @component Event Logs detail page
 *
 * @returns {JSX.Element}
 *
 */
export const EventlogDetails = (): JSX.Element => {
  const theme = useTheme();
  /**
   * State to manage order type
   */
  const [order, setOrder] = React.useState<Order>('asc');
  /**
   * State to manage order by column
   */
  const [orderBy] = React.useState<keyof Data>('value');
  /**
   * State to manage row selection
   */
  const [selected, setSelected] = React.useState<string[]>([]);
  /**
   * State to manage row selection
   */
  const [page, setPage] = React.useState(0);
  /**
   * State to manage row selection
   */
  const [rowsPerPage, setRowsPerPage] = React.useState(5);
  const emptyRows = rowsPerPage - Math.min(rowsPerPage, rows.length - page * rowsPerPage);

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
   * Function for handling error message and shown in red color
   *
   * @param {number} id  table row id
   *
   * @returns {object}
   *
   */
  function makeRed(id: number) {
    if (id === 3) return { color: theme.palette.error.main };
    return {};
  }

  return (
    <Grid>
      <SimpleTable
        order={order}
        setOrder={setOrder}
        selected={selected}
        setSelected={setSelected}
        page={page}
        setPage={setPage}
        rowsPerPage={rowsPerPage}
        setRowsPerPage={setRowsPerPage}
        headCells={headCells}
        rows={rows}
        footer="12:30 p.m. 07/14/2020"
      >
        {stableSort(rows, getComparator(order, orderBy))
          .slice(page * rowsPerPage, page * rowsPerPage + rowsPerPage)
          .map((row) => {
            return (
              <StyledTableRow
                hover
                onClick={(event: React.MouseEvent<unknown>) => handleClick(event, row.value)}
                tabIndex={-1}
                key={row.id}
              >
                <TableCell align="left" style={makeRed(row.id)}>
                  {row.value}
                </TableCell>
                <TableCell align="left" style={makeRed(row.id)}>
                  {row.measurement}
                </TableCell>
              </StyledTableRow>
            );
          })}
        {emptyRows > 0 && (
          <TableRow>
            <TableCell colSpan={6} />
          </TableRow>
        )}
      </SimpleTable>
    </Grid>
  );
};

export default EventlogDetails;
