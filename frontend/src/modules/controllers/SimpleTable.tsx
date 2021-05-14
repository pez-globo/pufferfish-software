/**
 * @summary Re-usable Component for Table
 *
 * @file Consits configuration for Pagination, Sorting, Row Selection
 */
/* eslint-disable @typescript-eslint/no-explicit-any */
import {
  Grid,
  Table,
  TableBody,
  TableCell,
  TableContainer,
  TableHead,
  TablePagination,
  TableRow,
  TableSortLabel,
  withStyles,
} from '@material-ui/core';
import { createStyles, makeStyles, Theme } from '@material-ui/core/styles';
import React, { PropsWithChildren, ReactNode } from 'react';
import { DECIMAL_RADIX } from '../app/AppConstants';

/**
 * @template T Sub comparator method
 *
 * @param {T} a Comparison object a
 * @param {T} b Comparison object b
 * @param {keyof T} orderBy column to be sorted by
 *
 * @return {number} Comparation result
 *
 */
function descendingComparator<T>(a: T, b: T, orderBy: keyof T) {
  if (b[orderBy] < a[orderBy]) {
    return -1;
  }
  if (b[orderBy] > a[orderBy]) {
    return 1;
  }
  return 0;
}

export type Order = 'asc' | 'desc';

/**
 * Comparator used for sorting
 * 
 * @param {Order} order Order type asc/desc
 * @param {Key} key Key for which to Order/Sort by
 *
 * @returns the output of [[descendingComparator]] Comparation result
 *
 */
export function getComparator<Key extends keyof number | string>(
  order: Order,
  orderBy: Key,
): (a: { [key in Key]: number | string }, b: { [key in Key]: number | string }) => number {
  return order === 'desc'
    ? (a, b) => descendingComparator(a, b, orderBy)
    : (a, b) => -descendingComparator(a, b, orderBy);
}

/**
 * @template T Wrapper for Sorting
 *
 * @param {T[]} array Rows of table
 * @param {function} comparator `getComparator` function
 *
 * @returns {T[]} Sorted Rows
 *
 */
export function stableSort<T>(array: T[], comparator: (a: T, b: T) => number): T[] {
  const stabilizedThis = array.map((el, index) => [el, index] as [T, number]);
  stabilizedThis.sort((a, b) => {
    const order = comparator(a[0], b[0]);
    if (order !== 0) return order;
    return a[1] - b[1];
  });
  return stabilizedThis.map((el) => el[0]);
}

const useStyles = makeStyles((theme: Theme) =>
  createStyles({
    tableRowStyle: {},
    tableContainer: {
      width: '100%',
      border: `2px dashed ${theme.palette.background.default}`,
      borderRadius: theme.panel.borderRadius,
      flexGrow: 1,
    },
    table: {
      minWidth: 500,
      padding: '3px solid black',
      backgroundColor: theme.palette.background.paper,
      '& tbody': {
        '& tr': {
          '& th': {
            padding: '12px 16px',
          },
          '& td': {
            padding: '12px 16px',
          },
        },
      },
    },
    visuallyHidden: {
      border: 0,
      clip: 'rect(0 0 0 0)',
      height: 1,
      margin: -1,
      overflow: 'hidden',
      padding: 0,
      position: 'absolute',
      top: 20,
      width: 1,
    },
    typeWrapper: {
      width: '50%',
      textAlign: 'center',
      borderRadius: 3,
      padding: 4,
    },
    footer: {
      fontFamily: 'sans-serif',
      fontSize: 16,
      marginTop: 15,
      fontWeight: 100,
    },
    tableContainerStyle: {
      position: 'absolute',
      width: '100%',
      top: '0px',
      marginBottom: 'auto',
      left: 0,
      padding: '20px 24px 8px',
      height: '100%',
      display: 'flex',
    },
  }),
);

/**
 * @typedef HeadCell
 *
 * Interface for Header Cell
 *
 * @prop {boolean} disablePadding Dynamic styling used to decide content padding in row cell
 * @prop {string | number} id Unique Identifier of column
 * @prop {string} label Column label shows in UI
 * @prop {boolean} numeric Column type
 * @prop {boolean} enableSort Toggle for sorting feature at column level
 */
export interface HeadCell {
  disablePadding: boolean;
  id: string | number;
  label: string;
  numeric: boolean;
  enableSort: boolean;
}

/**
 * @typedef EnhancedTableProps
 *
 * Props interface for Enhanced table
 *
 * @prop {ReturnType<typeof useStyles>} classes Css Class applied to row header
 * @prop {number} numSelected Number of row selected
 * @prop {function} onRequestSort Callback when Sorting by clicking on row header
 * @prop {function} onSelectAllClick Callback on All row selection
 * @prop {Order} order Order type asc/desc
 * @prop {string} orderBy Order column identifier
 * @prop {number} rowCount Count of rows
 * @prop {HeadCell[]} headCells list of all columns coonfigured in table
 */
export interface EnhancedTableProps {
  classes: ReturnType<typeof useStyles>;
  numSelected: number;
  onRequestSort: (event: React.MouseEvent<unknown>, property: string | number) => void;
  onSelectAllClick: (event: React.ChangeEvent<HTMLInputElement>) => void;
  order: Order;
  orderBy?: string;
  rowCount: number;
  headCells: HeadCell[];
}

export const StyledTableRow = withStyles((theme: Theme) =>
  createStyles({
    root: {
      '&:nth-of-type(odd)': {
        backgroundColor: theme.palette.primary.dark,
      },
    },
  }),
)(TableRow);

const StyledTableCell = withStyles((theme: Theme) =>
  createStyles({
    head: {
      backgroundColor: theme.palette.background.paper,
      borderBottom: `1px solid${theme.palette.text.primary}`,
    },
    body: {
      fontSize: 14,
    },
  }),
)(TableCell);

/**
 * Table Header
 * 
 * @param {EnhancedTableProps} props - Configuration for Table Header
 *
 * @returns JSX.Element
 *
 */
function EnhancedTableHead(props: EnhancedTableProps) {
  const { classes, order, orderBy, onRequestSort, headCells } = props;
  const createSortHandler = (property: string | number) => (event: React.MouseEvent<unknown>) => {
    onRequestSort(event, property);
  };

  return (
    <TableHead>
      <TableRow className={classes.tableRowStyle}>
        {headCells.map((headCell: HeadCell) => (
          <StyledTableCell
            key={headCell.id}
            align="left"
            padding="default"
            sortDirection={orderBy === headCell.id ? order : false}
          >
            {headCell.enableSort && orderBy ? (
              <TableSortLabel
                active={orderBy === headCell.id}
                direction={orderBy === headCell.id ? order : 'asc'}
                onClick={createSortHandler(headCell.id)}
              >
                {headCell.label}
                {orderBy === headCell.id ? (
                  <span className={classes.visuallyHidden}>
                    {order === 'desc' ? 'sorted descending' : 'sorted ascending'}
                  </span>
                ) : null}
              </TableSortLabel>
            ) : (
              headCell.label
            )}
          </StyledTableCell>
        ))}
      </TableRow>
    </TableHead>
  );
}

/**
 * @typedef TableProps
 *
 * Props interface for table
 *
 * @prop {Order} order Order type asc/desc
 * @prop {function} setOrder Callback on updating Order type
 * @prop {string} orderBy Order column identifier
 * @prop {function} setOrderBy Callback on updating Order column identifier
 * @prop {string[]} selected List of selected rows
 * @prop {function} setSelected Callback on selection of rows
 * @prop {number} page current page
 * @prop {function} setPage Callback on updating current page
 * @prop {number} rowsPerPage - Rows per page
 * @prop {function} setRowsPerPage - Configuration for setting number of rows per page
 * @prop {HeadCell[]} headCells list of all columns coonfigured in table
 * @prop {any[]} rows Row data
 * @prop {string | ReactNode} footer Table footer
 */
interface TableProps {
  order?: Order;
  setOrder(key: Order): void;
  orderBy?: string;
  setOrderBy?(orderBy: any): void;
  selected: string[];
  setSelected(rows: string[]): void;
  page?: number;
  setPage(page: number): void;
  rowsPerPage?: number;
  setRowsPerPage(count: number): void;
  headCells: HeadCell[];
  rows: any[];
  footer?: string | ReactNode;
}

/**
 * SimpleTable
 *
 * @component Simple table reusable component
 *
 * @returns JSX.Element
 */
export const SimpleTable = (props: PropsWithChildren<TableProps>): JSX.Element => {
  const {
    order = 'asc',
    orderBy,
    selected = [],
    page = 0,
    rowsPerPage = 8,
    setOrder,
    setOrderBy,
    setSelected,
    setPage,
    setRowsPerPage,
    rows,
    footer,
    headCells,
    children,
  } = props;
  const classes = useStyles();

  /**
   * Callback when Sorting by clicking on row header
   *
   * @param {React.MouseEvent<unknown>} event - DOM mouse Event
   * @param {string | number} property - Column to sort by
   *
   */
  const handleRequestSort = (event: React.MouseEvent<unknown>, property: string | number) => {
    const isAsc = orderBy === property && order === 'asc';
    setOrder(isAsc ? 'desc' : 'asc');
    if (orderBy && setOrderBy) {
      setOrderBy(property);
    }
  };

  /**
   * Callback when Sorting by clicking on row header
   *
   * @param {React.ChangeEvent<HTMLInputElement>} event - DOM Input change event
   *
   */
  const handleSelectAllClick = (event: React.ChangeEvent<HTMLInputElement>) => {
    if (event.target.checked) {
      const newSelecteds = rows.map((n) => n.id);
      setSelected(newSelecteds);
      return;
    }
    setSelected([]);
  };

  /**
   * Handles page change
   *
   * @param {unknown} event - DOM event
   * @param {number} newPage - Page number
   *
   */
  const handleChangePage = (event: unknown, newPage: number) => {
    setPage(newPage);
  };

  /**
   * Handles the number of rows to be displayed on each page
   *
   * @param {React.ChangeEvent<HTMLInputElement>} event - DOM input event
   *
   */
  const handleChangeRowsPerPage = (event: React.ChangeEvent<HTMLInputElement>) => {
    setRowsPerPage(parseInt(event.target.value, DECIMAL_RADIX));
    setPage(0);
  };

  return (
    <React.Fragment>
      <Grid item className={classes.tableContainerStyle} direction="column">
        <TableContainer className={classes.tableContainer}>
          <Table
            className={classes.table}
            aria-labelledby="tableTitle"
            size="medium"
            aria-label="enhanced table"
          >
            <EnhancedTableHead
              classes={classes}
              numSelected={selected.length}
              order={order}
              orderBy={orderBy}
              onSelectAllClick={handleSelectAllClick}
              onRequestSort={handleRequestSort}
              rowCount={rows.length}
              headCells={headCells}
            />
            <TableBody>{children}</TableBody>
          </Table>
        </TableContainer>

        <Grid container direction="row" justify="space-between">
          <Grid item className={classes.footer}>
            {footer}
          </Grid>
          <TablePagination
            rowsPerPageOptions={[8]}
            component="div"
            count={rows.length}
            rowsPerPage={rowsPerPage}
            page={page}
            onChangePage={handleChangePage}
            onChangeRowsPerPage={handleChangeRowsPerPage}
          />
        </Grid>
      </Grid>
    </React.Fragment>
  );
};

export default SimpleTable;
