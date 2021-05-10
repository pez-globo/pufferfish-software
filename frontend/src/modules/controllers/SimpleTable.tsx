/**
 * @summary A short one-line description for the file
 *
 * @file More detailed description for the file, if necessary;
 * perhaps spanning multiple lines.
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
 * @template T
 *
 * @param {T} a some description for a
 * @param {T} b some description for b
 * @param {keyof T} orderBy some description for orderBy
 *
 * @return {number} some description for the return value
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
 *
 * @param {Order} order some desc for order
 * @param {Key} orderBy some desc for orderBy
 *
 * @returns the output of [[descendingComparator]] function
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
 * @template T
 *
 * @param {T[]} array some desc for array
 * @param {function} comparator some desc for comparator
 *
 * @returns {T[]} some description for the return value
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
 * @prop {boolean} disablePadding desc for disablePadding
 * @prop {string | number} id desc for id
 * @prop {string} label desc for label
 * @prop {boolean} numeric desc for numeric
 * @prop {boolean} enableSort desc for enableSort
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
 * @prop {ReturnType<typeof useStyles>} classes desc for classes
 * @prop {number} numSelected desc for numSelected
 * @prop {function} onRequestSort desc for onRequestSort
 * @prop {function} onSelectAllClick desc for onSelectAllClick
 * @prop {Order} order desc for order
 * @prop {string} orderBy desc for orderBy
 * @prop {number} rowCount desc for rowCount
 * @prop {HeadCell[]} headCells desc for headCells
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
 * @param {EnhancedTableProps} props - desc for props
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
 * @prop {Order} order desc for order
 * @prop {function} setOrder desc for setOrder
 * @prop {string} orderBy desc for orderBy
 * @prop {function} setOrderBy desc for setOrderBy
 * @prop {string[]} selected desc for selected
 * @prop {function} setSelected desc for setSelected
 * @prop {number} page desc for page
 * @prop {function} setPage desc for setPage
 * @prop {number} rowsPerPage - number of rows to be displayed on each page
 * @prop {function} setRowsPerPage - sets the number of rows to be displayed on each page
 * @prop {HeadCell[]} headCells desc for headCells
 * @prop {any[]} rows desc for rows
 * @prop {string | ReactNode} footer desc for footer
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
   * some description
   *
   * @param {React.MouseEvent<unknown>} event - some desc for event
   * @param {string | number} property - some desc for property
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
   * some description
   *
   * @param {React.ChangeEvent<HTMLInputElement>} event - some desc for event
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
   * @param {unknown} event - some desc for event
   * @param {number} newPage - some desc for newPage
   *
   */
  const handleChangePage = (event: unknown, newPage: number) => {
    setPage(newPage);
  };

  /**
   * Handles the number of rows to be displayed on each page
   *
   * @param {React.ChangeEvent<HTMLInputElement>} event - some desc for event
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
