﻿// Copyright (c) Microsoft.  All Rights Reserved.
// Licensed under the Apache License, Version 2.0.  See License.txt in the project root for license information.

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Reflection;

namespace Microsoft.CodeAnalysis.Sarif
{
    /// <summary>
    /// The analysis tool that was run.
    /// </summary>
    public partial class Tool 
    {
        public static Tool CreateFromAssemblyData(string prereleaseInfo = null)
        {
            Assembly assembly = Assembly.GetEntryAssembly() ?? Assembly.GetExecutingAssembly();
            string name = Path.GetFileNameWithoutExtension(assembly.Location);

            Tool tool = new Tool();

            // 'name'
            tool.Name = name;

            // 'version' : primary tool version.
            Version version = assembly.GetName().Version;
            tool.Version = version.ToString();

            // Synthesized semver 2.0 version required by spec
            tool.SemanticVersion = version.Major.ToString() + "." + version.Minor.ToString() + "." + version.Build.ToString();

            // Binary file version
            FileVersionInfo fileVersion = FileVersionInfo.GetVersionInfo(assembly.Location);
            tool.FileVersion = fileVersion.FileVersion;

            tool.FullName = name + " " + tool.Version + (prereleaseInfo ?? "");

            tool.Properties = new Dictionary<string, string>();

            tool.Language = CultureInfo.CurrentCulture.Name;

            if (!string.IsNullOrEmpty(fileVersion.Language)) { tool.Properties["Language"] = fileVersion.Language; };
            if (!string.IsNullOrEmpty(fileVersion.Comments)) { tool.Properties["Comments"] = fileVersion.Comments; };
            if (!string.IsNullOrEmpty(fileVersion.CompanyName)) { tool.Properties["CompanyName"] = fileVersion.CompanyName; };
            if (!string.IsNullOrEmpty(fileVersion.ProductName)) { tool.Properties["ProductName"] = fileVersion.ProductName; };

            if (!string.IsNullOrEmpty(fileVersion.ProductVersion) && fileVersion.ProductVersion != tool.Version)
            {
                tool.Properties["ProductVersion"] = fileVersion.ProductVersion;
            };

            if (tool.Properties.Count == 0) { tool.Properties = null; }

            return tool;
        }
    }
}
