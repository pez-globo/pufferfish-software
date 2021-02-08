import React from 'react';
import SvgIcon, { SvgIconProps } from '@material-ui/core/SvgIcon';
import { makeStyles, Theme } from '@material-ui/core/styles';

const useStyles = makeStyles((theme: Theme) => ({
  path: {
    color: theme.palette.text.primary,
  },
}));

const PowerChargingIcon = (props: SvgIconProps): JSX.Element => {
  const classes = useStyles();
  return (
    <SvgIcon {...props} width="73" height="19" viewBox="0 0 50 19">
      <g
        id="Battery_charging_"
        data-name="Battery (charging)"
        transform="translate(19200.002 13518.009)"
      >
        <path
          id="Exclusion_9"
          data-name="Exclusion 9"
          d="M12.527,16.028h-.656l0-.019h.666l-.014.019Zm-.652-.018H3a3,3,0,0,1-3-3v-10a3,3,0,0,1,3-3H15.233l-.86,1.421h1.564L16.2.007H24a3,3,0,0,1,3,3v10a3,3,0,0,1-3,3H12.541L13.6,14.581h10a2,2,0,0,0,2-2V3.439a2,2,0,0,0-2-2H3.5a2,2,0,0,0-2,2v9.14a2,2,0,0,0,2,2h8.689l-.311,1.428ZM13.6,14.581H12.186l0-.008H13.61l-.006.008ZM23,13.009H4a1,1,0,0,1-1-1v-8a1,1,0,0,1,1-1h9.321L9.073,9.73h4.11L12.488,13h2.283l4.435-5.785H14.945c0-.018.346-1.989.726-4.2H13.323L13.328,3h2.345v.008H23a1,1,0,0,1,1,1v8A1,1,0,0,1,23,13.009Zm6-2H28v-6h1a2,2,0,0,1,2,2v2A2,2,0,0,1,29,11.009ZM16.2.007h-.963l0-.007H16.2V.007Z"
          transform="translate(-19200.002 -13518.009)"
          className={classes.path}
        />
      </g>
    </SvgIcon>
  );
};

export default PowerChargingIcon;
