﻿using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Web;

namespace WebComponent.Models
{
    public class ConfigFormModel
    {
        [DataType(DataType.Date)]
        [DisplayFormat(DataFormatString = "{0:dd-MM-yyyy}", ApplyFormatInEditMode = true)]
        public DateTime DesiredDate { get; set; }

        [DataType(DataType.Text)]
        public string TauxChange { get; set; }

    }
}