Name:		libax_core
Version:	1.1
Release:	1%{?dist}
Summary:	"A-Cross" C++ Essential Framework

Group:		Development
License:	New BSD License
URL:		libax.googlecode.com
Source0:	libax_core-%{version}.tar.gz
BuildRoot:	%(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)

BuildRequires:	gcc
Requires:	gcc

%description


%prep
%setup -q


%package devel
Group:		Development/Libraries
Summary:	Development files for libax
Requires:	gcc

%description devel
Development files for libax

%build
%configure
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT


%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%doc


%files devel
%{_includedir}/*
%{_libdir}/*

%changelog

