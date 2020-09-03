import React from 'react'
import { createStyles, makeStyles, Theme, useTheme } from '@material-ui/core/styles'
import {
    Grid,
    TableCell,
    TableRow,
    Typography
} from '@material-ui/core'
import SimpleTable, { stableSort, getComparator, StyledTableRow, Order } from '../controllers/SimpleTable'
import ModalPopup from '../controllers/ModalPopup'
import EventlogDetails from './container/EventlogDetails'

/**
 * LogsPage
 * 
 * TODO: Abstract this into components!
 */

interface Data {
    type: string,
    alarm: string,
    time: string, // Note: Make this a date object?
    details: string // Note: Make this an ID to view more details?,
    id: number
}

function createData(
    type: string,
    alarm: string,
    time: string,
    details: string,
    id: number
): Data {
    return { type, alarm, time, details, id }
}

const rows = [
    createData('Operator', 'Peep above upper limit', '12:30 p.m. 07/14/2020', 'View Details', 1),
    createData('System', 'Peep above upper limit', '12:10 p.m. 07/14/2020', 'View Details', 2),
    createData('Patient', 'Peep above upper limit', '12:00 p.m. 07/14/2020', 'View Details', 3),
    createData('System', 'Peep above upper limit', '1:30 p.m. 07/14/2020', 'View Details', 4),
    createData('Operator', 'Peep above upper limit', '9:30 p.m. 07/14/2020', 'View Details', 5),
    createData('System', 'Peep above upper limit', '10:30 p.m. 07/14/2020', 'View Details', 6),
    createData('Patient', 'Peep above upper limit', '12:30 p.m. 07/14/2020', 'View Details', 7),
    createData('Patient', 'Peep above upper limit', '12:30 p.m. 07/14/2020', 'View Details', 8),
    createData('Patient', 'Peep above upper limit', '12:30 p.m. 07/14/2020', 'View Details', 9),
    createData('Patient', 'Peep above upper limit', '12:30 p.m. 07/14/2020', 'View Details', 10),
]

interface HeadCell {
    disablePadding: boolean,
    id: keyof Data,
    label: string,
    numeric: boolean
}

const headCells: HeadCell[] = [
    { id: 'type', numeric: false, disablePadding: true, label: 'Type' },
    { id: 'alarm', numeric: true, disablePadding: false, label: 'Alarm' },
    { id: 'time', numeric: true, disablePadding: false, label: 'Time/Date' },
    { id: 'details', numeric: false, disablePadding: false, label: 'Details' },
]


const useStyles = makeStyles((theme: Theme) =>
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
        }
    }),
)

/**
 * LogsPage
 */
export const LogsPage = () => {
    const classes = useStyles()
    const theme = useTheme()
    const [order, setOrder] = React.useState<Order>('asc')
    const [orderBy, setOrderBy] = React.useState<keyof Data>('type')
    const [selected, setSelected] = React.useState<any>([])
    const [page, setPage] = React.useState(0)
    const [rowsPerPage, setRowsPerPage] = React.useState(8)
    const [open, setOpen] = React.useState(false)
    const [currentRow, setCurrentRow] = React.useState<Data>()
    const isSelected = (name: string) => selected.indexOf(name) !== -1
    const emptyRows = rowsPerPage - Math.min(rowsPerPage, rows.length - page * rowsPerPage)

    const handleClose = () => {
        setOpen(false)
    }

    const handleConfirm = () => {
        handleClose()
    }

    const typeColor = (type: string | undefined) => {
        if (type === 'Operator') return { backgroundColor: theme.palette.primary.main }
        else if (type === 'Patient') return { backgroundColor: '#92D25B', color: 'black' }
        else if (type === 'System') return { backgroundColor: '#E68619' }
        return { backgroundColor: '#E68619' }
    }

    const handleClick = (event: React.MouseEvent<unknown>, name: string) => {
        const selectedIndex = selected.indexOf(name)
        let newSelected: string[] = []

        if (selectedIndex === -1) {
            newSelected = newSelected.concat(selected, name)
        } else if (selectedIndex === 0) {
            newSelected = newSelected.concat(selected.slice(1))
        } else if (selectedIndex === selected.length - 1) {
            newSelected = newSelected.concat(selected.slice(0, -1))
        } else if (selectedIndex > 0) {
            newSelected = newSelected.concat(
                selected.slice(0, selectedIndex),
                selected.slice(selectedIndex + 1),
            )
        }
        setSelected(newSelected)
    }

    return (
        <Grid
            container
            direction="column"
            justify="space-between"
            alignItems="stretch"
            className={classes.root}
        >
            <Grid item>
                <Typography variant='h3'>Events Log</Typography>
            </Grid>

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
                        const isItemSelected = isSelected(row.type)
                        const labelId = `enhanced-table-checkbox-${index}`

                        return (
                            <StyledTableRow
                                hover
                                onClick={(event: any) => handleClick(event, row.type)}
                                tabIndex={-1}
                                key={row.details}
                            >
                                <TableCell align='left' component="th" id={labelId} scope="row" >
                                    <Grid
                                        className={classes.typeWrapper}
                                        style={typeColor(row.type)}>
                                        {row.type}
                                    </Grid>
                                </TableCell>
                                <TableCell align='left' component="th" id={labelId} scope="row" >
                                    {row.alarm}
                                </TableCell>
                                <TableCell align="left">{row.time}</TableCell>
                                <TableCell align="left" onClick={() => { setCurrentRow(row); setOpen(true) }}>{row.details}</TableCell>
                            </StyledTableRow>
                        )
                    })}
                {emptyRows > 0 && (
                    <TableRow style={{ height: 53 * emptyRows }}>
                        <TableCell colSpan={6} />
                    </TableRow>
                )}
            </SimpleTable>
            <ModalPopup showCloseIcon={true}
                label={
                    (<Grid>
                        <Grid
                            className={classes.typeWrapper}
                            style={typeColor(currentRow?.type)}>
                            {currentRow?.type}
                        </Grid>
                        <Grid>
                            <Typography variant="h4">{` PEEP Above upper limit`}</Typography>
                        </Grid>
                    </Grid>
                    )
                }
                open={open} onClose={handleClose} onConfirm={handleConfirm}>
                <Grid item>
                    <EventlogDetails />
                </Grid>
            </ModalPopup>
        </Grid>
    )
}

export default LogsPage